#include <cconf/config.h>
#include <cconf/token.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_delim(char c) {
	return c == '[' || c == ']' || c == '=' || c == ' ' || c == '\t' || c == '\n';
}

int is_delim_token(char c) {
	return c == '[' || c == ']' || c == '=';
}

void pass_comment(char **buffer) {
	for (*buffer += 1; **buffer && **buffer != '\n'; *buffer += 1)
		;
}

void pass_string(char **buffer) {
	for (*buffer += 1; **buffer && **buffer != '\n' && **buffer != '\"'; *buffer += 1)
		;
}

void parse_string(token_t *token, char **buffer) {
	token_create(token);
	char *last = *buffer + 1;
	for (*buffer += 1; **buffer && **buffer != '\n' && **buffer != '\"'; *buffer += 1)
		;
	token_set_string_value(token, last, *buffer);
}

size_t precount_tokens(char *buffer) {
	char *curr = buffer, *last = buffer;
	size_t cnt = 0;
	while (*curr) {
		if (*curr == '#') {
			pass_comment(&curr);
			last = curr + 1;
		} else if (*curr == '\"') {
			pass_string(&curr);
			++cnt;
			last = curr + 1;
		} else if (is_delim(*curr)) {
			if (curr - last) {
				++cnt;
			}
			if (is_delim_token(*curr)) {
				++cnt;
			}
			last = curr + 1;
		}
		++curr;
	}
	if (curr - last) {
		++cnt;
	}
	return cnt;
}

int parse_tokens(token_t *tokens, char *buffer) {
	char *curr = buffer, *last = buffer;
	size_t cnt = 0, section_cnt = 1;
	while (*curr) {
		if (*curr == '#') {
			pass_comment(&curr);
			last = curr + 1;
		} else if (*curr == '\"') {
			//pass_string(&curr); -> TODO
			parse_string(&tokens[cnt], &curr);
			++cnt;
			last = curr + 1;
		} else if (is_delim(*curr)) {
			if (curr - last) {
				token_create(&tokens[cnt]);
				token_set_string_value(&tokens[cnt], last, curr);
				// printf("Value: %s\n", tokens[cnt].value.s);
				++cnt;
			}
			if (is_delim_token(*curr)) {
				token_create(&tokens[cnt]);
				token_set_char_value(&tokens[cnt], *curr);
				if (tokens[cnt].type == token_type_lbracket) {
					++section_cnt;
				}
				++cnt;
			}
			last = curr + 1;
		}
		++curr;
	}
	if (curr - last) {
		token_create(&tokens[cnt]);
		token_set_string_value(&tokens[cnt], last, curr);
		// printf("Value: %s\n", tokens[cnt].value.s);
		++cnt;
	}
	return section_cnt;
}

void config_create(config_t *config, char *fn) {
	config->sections = 0;
	config->section_count = 0;

	char *buffer = 0;
	long len;
	FILE *file = fopen(fn, "rb");

	if (file) {
		fseek(file, 0, SEEK_END);
		len = ftell(file);
		fseek(file, 0, SEEK_SET);
		buffer = (char *) malloc(sizeof(char) * (len + 1));
		if (buffer) {
			fread(buffer, sizeof(char), len, file);
		}
		fclose(file);
		buffer[len] = 0;
	}
	if (buffer) {
		size_t token_count = precount_tokens(buffer);
		token_t *tokens = (token_t *) malloc(sizeof(token_t) * token_count);
		size_t section_counter = 0, pair_counter;
		config->section_count = parse_tokens(tokens, buffer);
		config->sections = (section_t *) malloc(sizeof(section_t) *
																						config->section_count);

		section_create(&config->sections[0], ""); // first section without a name
																							// global variables

		for (int i = 0; i < token_count; i++) {
			// printf("%d\n", tokens[i].type);
			if (tokens[i].type == token_type_lbracket) {
				++section_counter;
				// check next and the one after
				if (i + 2 < token_count && tokens[i + 1].type == token_type_string &&
						tokens[i + 2].type == token_type_rbracket) {
					section_create(&config->sections[section_counter],
												 tokens[i + 1].value.s);
					i += 2;
				} else {
					//err
				}
			} else if (tokens[i].type == token_type_equal) {
				++config->sections[section_counter].pair_count;
			}
		}

		for (int i = 0; i < config->section_count; i++) {
			config->sections[i].pairs = (pair_t *) malloc(sizeof(pair_t) *
																										config->sections[i].pair_count);
		}

		section_counter = 0, pair_counter = 0;

		for (int i = 0; i < token_count; i++) {
			// printf("%d\n", tokens[i].type);
			if (tokens[i].type == token_type_lbracket) {
				++section_counter;
				pair_counter = 0;
			} else if (tokens[i].type == token_type_equal) {
				if (i - 1 >= 0 && i + 1 < token_count) {
					// printf("Key: %s, Value: %s\n", tokens[i-1].value.s, tokens[i+1].value.s);
					pair_create(&config->sections[section_counter].pairs[pair_counter]);
					pair_set_key(&config->sections[section_counter].pairs[pair_counter],
											 tokens[i - 1].value.s);
					pair_set_value(&config->sections[section_counter].pairs[pair_counter],
												 tokens[i + 1].value.s);
					i += 1;
					pair_counter++;
				} else {
					// err
				}
			}
		}

		for (int i = 0; i < token_count; i++) {
			token_delete(&tokens[i]);
		}
		free(tokens);
		free(buffer);
	} else {
		// err...
	}
}

section_t *config_get_section(config_t *config, char *name) {
	for (int i = 0; i < config->section_count; i++) {
		if (strcmp(config->sections[i].name, name) == 0) {
			return &config->sections[i];
		}
	}
	return 0;
}

int config_to_file(config_t *config, FILE *out) {
	int ret = 0;
	if (out) {
		for (int i = 0; i < config->section_count; i++) {
			section_t *sec = &config->sections[i];
			if (strcmp(sec->name, "") != 0) {
				// normal section, for global dont output anything
				fprintf(out, "[%s]\n", sec->name);
			}
			// print keys and values
			for (int j = 0; j < sec->pair_count; j++) {
				pair_t *pair = &sec->pairs[j];
				fprintf(out, "%s = \"%s\"\n", pair->key, pair->value);
			}
			// skip last newline
			if (i != config->section_count - 1)
				fprintf(out, "\n");
		}
	} else {
		ret = -1;
	}
	return ret;
}

pair_t **config_get_pairs(config_t *config, char *key, size_t *count) {
	pair_t **pairs = 0;
	section_t *temp_sec = 0;
	size_t i, j, counter = 0;
	// precount
	for (i = 0; i < config->section_count; i++) {
		temp_sec = &config->sections[i];
		for (j = 0; j < temp_sec->pair_count; j++) {
			if (strcmp(temp_sec->pairs[j].key, key) == 0) {
				++counter;
			}
		}
	}
	pairs = (pair_t **) malloc(sizeof(pair_t *) * counter);
	for (i = 0, counter = 0; i < config->section_count; i++) {
		temp_sec = &config->sections[i];
		for (j = 0; j < temp_sec->pair_count; j++) {
			if (strcmp(temp_sec->pairs[j].key, key) == 0) {
				pairs[counter] = &temp_sec->pairs[i];
				++counter;
			}
		}
	}
	*count = counter;
	return pairs;
}

void config_delete(config_t *config) {
	int i;
	for (i = 0; i < config->section_count; i++) {
		section_delete(&config->sections[i]);
	}
	free(config->sections);
	config->section_count = 0;
}
#include <assert.h>
#include <getopt.h> // getopt_long
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define WORDS_INITIAL_SIZE 32

typedef struct Words {
  char **words;
  size_t length;
  size_t reserved_length;
} Words;

Words *new_words() {
  Words *words = malloc(sizeof(*words));
  if (words == NULL) {
    return NULL;
  }
  words->words = malloc(sizeof(*words->words) * WORDS_INITIAL_SIZE);
  if (words->words == NULL) {
    return NULL;
  }
  words->length = 0;
  words->reserved_length = WORDS_INITIAL_SIZE;
  return words;
}

bool add_word(Words *const words, const char *const word) {
  assert(words->length <= words->reserved_length &&
         "number of words in buffer must be less than the buffer size");
  // buffer expand logic
  if (words->length == words->reserved_length) {
    // double buffer length on realloc
    size_t next_reserved_length = words->reserved_length * 2;
    char **next_words =
        realloc(words->words, sizeof(*words->words) * next_reserved_length);
    // don't insert if out of memory
    if (next_words == NULL) {
      return false;
    }
    words->reserved_length = next_reserved_length;
    words->words = next_words;
  }
  // allows for passing of stack objects
  char *owned_word = malloc(sizeof(*word) * (strlen(word) + 1));
  if (owned_word == NULL) {
    return false;
  }
  strcpy(owned_word, word);
  words->words[words->length] = owned_word;
  words->length += 1;
  return true;
}

char *random_word(const Words *const words) {
  // NOTE: better randomness could be done but I don't think I need to
  return words->words[rand() % words->length];
}

void free_words(Words **words_ptr) {
  Words *words = *words_ptr;
  for (size_t i = 0; i < words->length; i += 1) {
    free(words->words[i]);
  }
  free(words->words);
  free(words);
  *words_ptr = NULL;
}

#define LINE_BUF_SZ 255

Words *load_words(const char *const filename) {
  assert(filename != NULL && "filename cannot be NULL");
  FILE *const file = fopen(filename, "r");
  if (file == NULL) {
    return NULL;
  }

  Words *words = new_words();
  if (words == NULL) {
    return NULL;
  }

  char line[LINE_BUF_SZ];
  while (fgets(line, LINE_BUF_SZ, file) != NULL) {
    // remove trailing newline
    line[strcspn(line, "\n")] = '\0';
    if (!add_word(words, line)) {
      return NULL;
    }
  }
  fclose(file);

  return words;
}

typedef struct Options {
  bool help;
  int lower;
  int upper;
  int count;
} Options;

const struct option LONG_OPTIONS[] = {
    {.name = "help",  .has_arg = no_argument, .flag = NULL, .val = 'h'},
    {.name = "count", .has_arg = required_argument, .flag = NULL, .val = 'c'},
    {.name = "lower", .has_arg = required_argument, .flag = NULL, .val = 'l'},
    {.name = "upper", .has_arg = required_argument, .flag = NULL, .val = 'u'},
    {0, 0, 0, 0}};

bool get_options(int argc, char **argv, Options *options) {
  *options = (Options){
      .help = false,
      .lower = 1,
      .upper = 5,
      .count = 1,
  };
  int c;
  while ((c = getopt_long(argc, argv, "l:u:c:h", LONG_OPTIONS, NULL)) != -1) {
    switch (c) {
    case 'h':
      options->help = true;
      break;
    case 'l':
      options->lower = atoi(optarg); // lazy
      break;
    case 'u':
      options->upper = atoi(optarg);
      break;
    case 'c':
      options->count = atoi(optarg);
      break;
    default:
      return false;
      break;
    }
  }
  return true;
}

const char *USAGE =
    "Usage: ineedideas [Option...]\n"
    "Options:\n"
    "  -l, --lower <NUM>\t\tThe lower bound on the number of words (default: "
    "1)\n"
    "  -u, --upper <NUM>\t\tThe upper bound on the number of words (default: "
    "5)\n"
    "  -c, --count <NUM>\t\tThe number of \"ideas\" to generate (default: 1)\n";

// utility macro to prevent repeated code for positive checks (bad?)
#define EXPECT_POSITIVE(var)                                                   \
  if (var < 0) {                                                               \
    fprintf(stderr, "%s must be greater than or equal to 0\n", #var);          \
    exit(1);                                                                   \
  }

int main(int argc, char **argv) {
  Options options;
  if (!get_options(argc, argv, &options)) {
    fprintf(stderr, "failed to parse options\n");
    return 1;
  }

  if (options.help) {
    printf(USAGE);
    return 0;
  }

  EXPECT_POSITIVE(options.count);
  EXPECT_POSITIVE(options.lower);
  EXPECT_POSITIVE(options.upper);

  if (options.lower > options.upper) {
    fprintf(stderr, "lower bound must be less than upper bound\n");
    return 1;
  }

  // stderr to allow redirection
  fprintf(stderr,
          "Generating %d with lower at least %d words and at most %d words\n",
          options.count, options.lower, options.upper);

  Words *words = load_words("words.txt");
  if (words == NULL) {
    fprintf(stderr, "failed to load words\n");
    return 1;
  }

  // set seed to current time
  srand(time(NULL));

  for (int i = 0; i < options.count; i += 1) {
    size_t num_words = rand() % (options.upper - options.lower) + options.lower;
    for (size_t i = 0; i < num_words; i += 1) {
      printf("%s ", random_word(words));
    }
    printf("\n");
  }

  free_words(&words);
  return 0;
}

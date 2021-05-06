#include <assert.h>
#include <getopt.h> // getopt_long
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "words.h"

const char *random_word(const char ** words, size_t num_words) {
  // NOTE: better randomness could be done but I don't think I need to
  return words[rand() % num_words];
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

  // set seed to current time
  srand(time(NULL));

  for (int i = 0; i < options.count; i += 1) {
    size_t num_words = rand() % (options.upper - options.lower) + options.lower;
    for (size_t i = 0; i < num_words; i += 1) {
      printf("%s ", random_word(WORDS, NUM_WORDS));
    }
    printf("\n");
  }

  return 0;
}

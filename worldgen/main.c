#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <sf/sf.h>
#include <voyager/voyager.h>


struct options
{
  sf_string_t name;
  struct vgr_hex_coordinate hex_coordinate;
};


static void
initialize_options(struct options *options);

static void
parse_options(int argc, char **argv, struct options *options);

static void
print_usage_and_exit(int argc, char **argv);


static void
initialize_options(struct options *options)
{
  options->name = sf_string("No Name");
  options->hex_coordinate = (struct vgr_hex_coordinate) { .horizontal=1, .vertical=1, };
}


int
main(int argc, char **argv)
{
  vgr_init();
  
  struct options options;
  initialize_options(&options);
  parse_options(argc, argv, &options);
  
  sf_random_t random = sf_random_from_random_state();
  vgr_world_t world = vgr_world(options.name, options.hex_coordinate, random, &random);
  
  sf_string_t description = sf_string_from(world);
  fprintf(stdout, "%s\n", sf_string_chars(description));
  
  vgr_fin();
  vgr_memory_expect_alloc_count_zero();
  return EXIT_SUCCESS;
}


static void
parse_options(int argc, char **argv, struct options *options)
{
  static struct option longOptions[] = {
    {
      .name = "help",
      .has_arg = no_argument,
      .flag = NULL,
      .val = 'h',
    },
    {
      .name = "name",
      .has_arg = required_argument,
      .flag = NULL,
      .val = 'n',
    },
    {
      .name = "hex",
      .has_arg = required_argument,
      .flag = NULL,
      .val = 'x',
    },
    {
      .name = NULL,
      .has_arg = no_argument,
      .flag = NULL,
      .val = 0,
    },
  };
  
  int longOptionIndex;
  int ch;
  while (-1 != (ch = getopt_long(argc, argv, "hn:x:", longOptions, &longOptionIndex))) {
    switch (ch) {
      case 'h':
        print_usage_and_exit(argc, argv);
        break;
      case 'n':
        options->name = sf_string(optarg);
        break;
      case 'x':
        {
          bool valid = vgr_hex_coordinate_from_string(optarg, &options->hex_coordinate);
          if ( ! valid) {
            fprintf(stderr, "ERROR: \"%s\" is not a valid hex coordinate\n", optarg);
            print_usage_and_exit(argc, argv);
          }
        }
        break;
      case '?':
        print_usage_and_exit(argc, argv);
        break;
      default:
        print_usage_and_exit(argc, argv);
        break;
    }
  }
}


static void
print_usage_and_exit(int argc, char **argv)
{
  fprintf(stderr,
          "usage: %s [OPTIONS]\n"
          "  -h, --help                       display this help message\n"
          "  -n, --name NAME                  set the world name\n"
          "  -x, --hex XXYY                   set the world hex coordinate\n"
          "                                   in the format \"0101\"\n"
          ,
          basename(argv[0])
          );
  sf_temp_pool_release();
  exit(EXIT_FAILURE);
}
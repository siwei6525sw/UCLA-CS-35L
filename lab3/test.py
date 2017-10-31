# !/usr/bin/python

import random, sys
import argparse
from optparse import OptionParser


class comm:
    def __init__(self, args, suppress_col_1, suppress_col_2, suppress_col_3, allow_unsorted):
        for arg in args:
            print(arg)

    def compare(self):
        print("Comparing!")


def main():
#    locale.setlocale(locale.LC_ALL, 'C')
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Output randomly selected lines from FILE."""

    args_parser = argparse.ArgumentParser(version=version_msg, usage=usage_msg)

    # define help messages
    suppress_col_1_help = "Suppress lines that are unique to FILE1 (column 1)"
    suppress_col_2_help = "Suppress lines that are unique to FILE2 (column 2)z"
    suppress_col_3_help = "Suppress lines that are contained in FILE1 and FILE2 (column 3)"
    allow_unsorted_help = "Allow for comparison of unsorted files"

    # add arguments to parser
    args_parser.add_argument("-1", action="store_true", dest="suppress_col_1", default=False, help=suppress_col_1_help)
    args_parser.add_argument("-2", action="store_true", dest="suppress_col_2", default=False, help=suppress_col_2_help)
    args_parser.add_argument("-3", action="store_true", dest="suppress_col_3", default=False, help=suppress_col_3_help)
    args_parser.add_argument("-u", action="store_true", dest="allow_unsorted", default=False, help=allow_unsorted_help)

    # parse system arguments, with first arg sliced
    options, args = args_parser.parse_args(sys.argv[1:])

    # check each argument
    try:
        allow_unsorted = bool(options.allow_unsorted)
    except:
        args_parser.error("Invalid argument for -u: {0}".format(options.allow_unsorted))

    try:
        suppress_col_1 = bool(options.suppress_col_1)
    except:
        args_parser.error("Invalid argument for -1: {0}".format(options.suppress_col_1))

    try:
        suppress_col_2 = bool(options.suppress_col_2)
    except:
        args_parser.error("Invalid argument for -2: {0}".format(options.suppress_col_2))

    try:
        suppress_col_3 = bool(options.suppress_col_3)
    except:
        args_parser.error("Invalid argument for -3: {0}".format(options.suppress_col_3))

    if len(args) != 2:
        args_parser.error("Missing operand")

    # if args[0] == "-" and args[0] == args[1]:
    #     parser.error("only one file can be read from stdin")

    try:
        generator = comm(args, suppress_col_1, suppress_col_2, suppress_col_3, allow_unsorted)
    except OSError as err:
        args_parser.error("Error: {0}".format(err))

    for arg in args:
        print("arg:")
        print(arg)
    print("supp: ")
    print(suppress_col_1)

    try:
        generator.compare()
    except:
        args_parser.error("Error")


if __name__ == "__main__":
    main()

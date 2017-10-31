# !/usr/bin/python

import random, sys
import argparse
from optparse import OptionParser


class comm:
    def __init__(self, arg1, arg2):
        # declare tuple array to print in form (line, column)
        self.to_print = []
        # initialize file1 and file2
        if(arg1 == "-"):
            self.file1 = sys.stdin.readlines()
        else:
            f = open(arg1, 'r')
            self.file1 = f.readlines()
            f.close()
        if(arg2 == "-"):
            self.file2 = sys.stdin.readlines()
        else:
            f = open(arg2, 'r')
            self.file2 = f.readlines()
            f.close()

    def compare(self, allow_unsorted):
        if allow_unsorted:
            self.unsorted_compare()
        else:
            self.sorted_compare()

    def sorted_compare(self):
        # compare files and add each line to appropriate column
        file1_index, file2_index = 0, 0
        while(file1_index < len(self.file1) and file2_index < len(self.file2)):
            if(self.file1[file1_index] == self.file2[file2_index]):
                self.to_print.append((self.file1[file1_index], 3))
                file1_index, file2_index = file1_index + 1, file2_index + 1
            elif(self.file1[file1_index] < self.file2[file2_index]):
                self.to_print.append((self.file1[file1_index], 1))
                file1_index += 1
            else:
                self.to_print.append((self.file2[file2_index], 2))
                file2_index += 1

        # possibly add the rest of one of the files to appropriate columns
        self.to_print.extend([(x, 1) for x in self.file1[file1_index:]])
        self.to_print.extend([(x, 2) for x in self.file2[file2_index:]])

    def unsorted_compare(self):
        for file1_line in self.file1:
            match = False
            for file2_line in self.file2:
                # found match in file 2: add to col 3, remove matche dline
                if file1_line == file2_line:
                    self.to_print.append((file1_line, 3))
                    self.file2.remove(file2_line)
                    match = True
                    break
            if not match:
                # no match in file 2: add to col 1
                self.to_print.append((file1_line, 1))
        # remaining lines in file 2 are unique to file 2: add to col 2
        self.to_print.extend([(x, 2) for x in self.file2])

    def print_out(self, suppress_col_1, suppress_col_2, suppress_col_3):
        # calculate appropriate number of tabs in column
        num_tabs, col_2_tabs, col_3_tabs = 0, 0, 0
        if not suppress_col_1:
            num_tabs += 1
        if not suppress_col_2:
            col_2_tabs = num_tabs
            num_tabs += 1
        if not suppress_col_3:
            col_3_tabs = num_tabs

        for line, col in self.to_print:
            # check for suppression and print
            if col == 1 and not suppress_col_1:
                sys.stdout.write(line)
            if col == 2 and not suppress_col_2:
                sys.stdout.write(col_2_tabs * "\t" + line)
            if col == 3 and not suppress_col_3:
                sys.stdout.write(col_3_tabs * "\t" + line)
def main():
    # define version/usage messages
    version_msg = "%prog 1.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2

Select or reject lines common to two files FILE1 and FILE2."""

    option_parser = OptionParser(usage=usage_msg, version=version_msg)

    # define help messages
    suppress_col_1_help = "Suppress lines that are unique to FILE1 (column 1)"
    suppress_col_2_help = "Suppress lines that are unique to FILE2 (column 2)z"
    suppress_col_3_help = "Suppress lines that are contained in FILE1 and FILE2 (column 3)"
    allow_unsorted_help = "Allow for comparison of unsorted files"

    # add arguments to parser
    option_parser.add_option("-1", action="store_true", dest="suppress_col_1", default=False, help=suppress_col_1_help)
    option_parser.add_option("-2", action="store_true", dest="suppress_col_2", default=False, help=suppress_col_2_help)
    option_parser.add_option("-3", action="store_true", dest="suppress_col_3", default=False, help=suppress_col_3_help)
    option_parser.add_option("-u", action="store_true", dest="allow_unsorted", default=False, help=allow_unsorted_help)

    # parse system arguments/options, with first arg sliced
    options, args = option_parser.parse_args(sys.argv[1:])

    if len(args) != 2:
        option_parser.error("Must have two files to compare")

    suppress_col_1 = bool(options.suppress_col_1)
    suppress_col_2 = bool(options.suppress_col_2)
    suppress_col_3 = bool(options.suppress_col_3)
    allow_unsorted = bool(options.allow_unsorted)

    try:
        # initialize comparer with the two file arguments
        comparer = comm(args[0], args[1])
    except OSError as err:
        option_parser.error("Argument error: {0}".format(err))

    comparer.compare(allow_unsorted)
    comparer.print_out(suppress_col_1, suppress_col_2, suppress_col_3)

if __name__ == "__main__":
    main()

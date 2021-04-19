import argparse

epilog ='''
Usage Examples:

    Read from COM4 using baudrate 9600 :
        python main.py -f 'com4' -s -b 9600
        
    Read from COM4 using baudrate 9600 and save to file data/output.csv:
        python main.py -f 'com4' -s -b 9600
        
    Read from a file: 
        python main.py -f <filename> 
'''
argparser = argparse.ArgumentParser(description="Program to use data over Serial to draw a realtime heatmap  "
    ,epilog=epilog
    ,formatter_class=argparse.RawDescriptionHelpFormatter
    )
argparser.add_argument("-f", "--filename", required=True, type=str)

argparser.add_argument(
    "-s",
    "--serial",
    action="store_true",
    help="reads from  Serial Port specified by -f",
)

argparser.add_argument(
    "-b",
    "--baudrate",
    type=int,
    default=2000000,
    help="sets the baudrate for the Serial Port (default : 115200)",
)
argparser.add_argument(
    "-r", "--rows", type=int, default=12, help="sets the number of rows.(default : 5)"
)
argparser.add_argument(
    "-c", "--cols", type=int, default=12, help="sets the number of columns.(default : 5)"
)
argparser.add_argument(
    "-o",
    "--outfile",
    type=str,
    default="",
    help="The file to store the recorded values.",
)
argparser.add_argument(
    "-oM",
    "--mode",
    choices=["w", "a"],
    default="w",
    help="Specifies the mode in which file specified by -o will be opened (default : w)",
)
argparser.add_argument(
    "-D",
    "--no_draw",
    action="store_true",
    help="do not draw the heatmap.",
)
argparser.add_argument(
    "-Mn",
    "--v_min",
    type=int,
    default = 600,
    help="set the min threshold for the heatmap (default : 600).",
)
argparser.add_argument(
    "-Mx",
    "--v_max",
    type=int,
    default = 1023,
    help="set the max threshold for the heatmap (default : 1023).",
)
argparser.add_argument(
    "-m",
    "--cmap",
    type=str,
    default = 'rainbow',
    choices=["rainbow",'gist_rainbow','hsv'],
    help="set the max threshold for the heatmap (default : 'rainbow').",
)
# parsed = argparser.parse_args()

# print(parsed)


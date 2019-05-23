#!/usr/bin/env python3

Description = """
Tool to extract sequences from a fasta file optionally limiting
sequence length, number of sequences, total size.
Also report maximum minimum and average sequence length

Format of output file can be controlled using the --before and --after options
Default is single line fasta format
  >
  seq1
  >
  seq2
  etc
but one can get the concatenated sequences setting --before="" --after=""
Note: in --before and --after you can use the python escape sequence "\\xhh"
to denote a byte with hex value hh. For example, to add a 0x00 byte at the
begining of each sequence and a 0x01 value followed by a \n at the end of
each sequence use --before="\\x00" --after="\\x01\\n". The resulting output
format will be
  0x00 seq1 0x01
  0x00 seq2 0x01
  0x00 seq3 0x01
  etc 
"""


import sys, argparse

 

def show_command_line(f):
  f.write("Command line: ") 
  for x in sys.argv:
     f.write(x+" ")
  f.write("\n")   


def main():
  show_command_line(sys.stderr)
  parser = argparse.ArgumentParser(description=Description, formatter_class=argparse.RawTextHelpFormatter)
  parser.add_argument('infile', help='input file name', type=str)
  parser.add_argument('outfile', nargs='?', help='output file name', default=None)
  parser.add_argument('-s', help="# initial seqs to be skipped (def 0)",  default=0, type=int)
  parser.add_argument('-n', help="# seqs to extract (def all)", default=-1, type=int)
  parser.add_argument('-m', help="output size limit in MB (def no limit)", default=-1, type=int)
  parser.add_argument('-l', help="force all seqs of this length (def no)", default=-1, type=int)
  parser.add_argument('-p', help="split single sequences (only with -l)", action="store_true")
  parser.add_argument('-N', help="discard reads containing N bases", action="store_true")
  parser.add_argument('--before', help='string to prepend to each sequence (def ">\\n")', default=">\n", type=str)
  parser.add_argument('--after', help='string to appped to each sequence (def "\\n")', default="\n", type=str)
  parser.add_argument('-v', help="verbose", action='store_true')
  args = parser.parse_args()
  # --- convert \n and \x in before/after
  # args.before = args.before.replace("\\n","\n")
  args.before = bytes(args.before, "utf-8").decode("unicode_escape")
  args.after = bytes(args.after, "utf-8").decode("unicode_escape")
  # --- init counters
  args.doc_written = 0
  args.doc_total_size = 0
  args.doc_skip = args.s
  args.doc_remaining = args.n
  args.maxv = 0
  args.minv = 5*10**9
  if args.outfile:
    g = open(args.outfile,"w")
  else:
    g = None    
  
  with open(args.infile) as f:
    status = 0
    firstline =True
    doc = ""
    for s in f:
      if s.startswith('>'): # end of previous document
        if firstline:
          firstline = False
          continue         # the first '>' has not previous document 
        assert len(doc)>0, "Null input sequence"
        processdoc(doc,args,g)
        doc = ""
        if args.doc_remaining == 0 or (args.m >0 and  args.doc_total_size >= args.m*1024*1024):
          break
      else: # inside a multiline sequence
        doc += s.rstrip()
    # process last sequence    
    else:
        processdoc(doc,args,g)
  # done. f is closed automatically 
  if args.outfile:
    g.close()
  print("Sequences produced:",args.doc_written)
  print("Total collection size:", args.doc_total_size, "(%f MBs)" % (args.doc_total_size/(2**20)) )
  print("Min Max Ave lenghts: ", args.minv, args.maxv, args.doc_total_size/args.doc_written)
  return        


# process a single read
def processdoc(doc,args,g):
  if args.N and "N" in doc: # discard docs with Ns
    return
  if args.l <= 0:         # no limit on size: output single read
    outdoc(doc,args,g)
  # we only want sequences of length exactly args.l  
  elif not (args.p):      # do not split
    if len(doc)>=args.l:  # output something only if length >= args.l  
      outdoc(doc[0:args.l],args,g)
  # split sequences to length exactly args.l and output them until is possible 
  else:
    while len(doc)>= args.l:
      d = doc[0:args.l]
      outdoc(d,args,g)
      doc = doc[args.l:]


# output a single document, updating statistics
def outdoc(doc,args,g):
  if args.doc_skip>0:
    args.doc_skip -= 1
    return 
  if args.doc_remaining == 0 or (args.m >0 and  args.doc_total_size >= args.m*1024*1024):
    return # do nothing
  size = len(doc)    
  args.doc_total_size += size
  if(size>args.maxv): args.maxv=size
  if(size<args.minv): args.minv=size
  if args.outfile:
    # default is output in fasta format, but can be changed by command line parameters
    print(args.before+doc,file=g,end=args.after) 
  args.doc_written += 1
  args.doc_remaining -=1
  

if __name__ == '__main__':
  main()

#usage: sh xxd.sh 文件 起始地址 长度 
xxd -u -a -g 1 -s $2 -l $3 $1 

#-u  use upper case hex letters. Default is lower case.
#
#-a | -autoskip
#	    toggle autoskip: A single ’*’ replaces nul-lines.  Default off.
#
#-g bytes | -groupsize bytes
#     separate the output of every <bytes> bytes (two hex characters or eight bit-digits each) by a whitespace.  Specify -g 0 to
#    suppress grouping.  <Bytes> defaults to 2 in normal mode and 1 in bits mode.  Grouping does not  apply  to  postscript  or
#    include style.
#
#-c cols | -cols cols
#            format <cols> octets per line. Default 16 (-i: 12, -ps: 30, -b: 6). Max 256.
#
#-s [+][-]seek
#   start at <seek> bytes abs. (or rel.) infile offset.  + indicates that the seek is relative to the current stdin file position 
#   (meaningless when not reading from stdin).  - indicates that the seek should be that many characters from the end of
#   the input (or if combined with +: before the current stdin file position).  
#   Without -s option, xxd starts at  the  current file position.

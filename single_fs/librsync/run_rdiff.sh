# !/bin/sh
this_path=`pwd`

export LD_LIBRARY_PATH=$this_path/:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=$this_path/../lib:${LD_LIBRARY_PATH}
echo ./rdiff_so tmp/part tmp/part1 tmp/sig tmp/delta tmp/result
./rdiff_so tmp/part tmp/sig tmp/part1 tmp/delta tmp/result


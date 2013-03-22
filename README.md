#Interleave

[![Build Status](https://travis-ci.org/gingi/interleave.png)](https://travis-ci.org/gingi/interleave)

Interleaves huge files together. Useful for combining mate-pair FASTQ files.

##Usage

`interleave` takes as input 2 or more text files, and outputs to a file specified with the `-o` option (or standard output).

    interleave [args] <file1> <file2> ...
    
    args:
       -l [num]   - Lines from each file to interleave (default: 4)
       -o [file]  - File to output

##Installation

    ./configure
    make
    make check
    make install

##Examples

Interleave 2 FASTQ mate-pair files:

    > cat FC42GXXXXXX_1.fastq
    @ROVE_0001:7:1:6:1934#0/1
    ATCGTCGATAGATCGGAAGAGCGGTTCAGCAGGAAT
    +ROVE_0001:7:1:6:1934#0/1
    gggfggggggggggggggggeggggggggcggchgg
    @ROVE_0001:7:1:6:54#0/1
    ATCGTCGATAGATCGGAAGAGCGGTTCAGCAGGAAT
    +ROVE_0001:7:1:6:54#0/1
    gggggggggggghggf`gggggggffgcfgggggge
    
    > cat FC42GXXXXXX_2.fastq
    @ROVE_0001:7:1:6:1934#0/2
    ATCGACGATAGATCGGAAGAGCGTCGTGTANGGAAA
    +ROVE_0001:7:1:6:1934#0/2
    ggeegedggggggggfgeggfgfggdbdbdEdTdb`
    @ROVE_0001:7:1:6:54#0/2
    ATCGACGATAGATCGGAAGAGCGTCGTGTANGGAAA
    +ROVE_0001:7:1:6:54#0/2
    gggggggcgggggggggggggggggggggcEffee^
    
    > ./interleave FC42GXXXXXX_1.fastq FC42GXXXXXX_2.fastq
    @ROVE_0001:7:1:6:1934#0/1
    ATCGTCGATAGATCGGAAGAGCGGTTCAGCAGGAAT
    +ROVE_0001:7:1:6:1934#0/1
    gggfggggggggggggggggeggggggggcggchgg
    @ROVE_0001:7:1:6:1934#0/2
    ATCGACGATAGATCGGAAGAGCGTCGTGTANGGAAA
    +ROVE_0001:7:1:6:1934#0/2
    ggeegedggggggggfgeggfgfggdbdbdEdTdb`
    @ROVE_0001:7:1:6:54#0/1
    ATCGTCGATAGATCGGAAGAGCGGTTCAGCAGGAAT
    +ROVE_0001:7:1:6:54#0/1
    gggggggggggghggf`gggggggffgcfgggggge
    @ROVE_0001:7:1:6:54#0/2
    ATCGACGATAGATCGGAAGAGCGTCGTGTANGGAAA
    +ROVE_0001:7:1:6:54#0/2
    gggggggcgggggggggggggggggggggcEffee^
    

Interleave 2 text files, taking one line from each at a time.

    > cat left-file.txt
    A
    B
    > cat right-file.txt
    X
    Y
    > ./interleave -l 1 -o merged.txt left-file.txt right-file.txt
    > cat merged.txt
    A
    X
    B
    Y



## License

    Copyright (c) 2012-2013 Shiran Pasternak <shiranpasternak at gmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions: The above
    copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

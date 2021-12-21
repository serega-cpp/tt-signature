Project Signature (test task for the Software Developer position interview)
==========================================================================

Generates the signature (CRC32) for each block of the source file and puts
it into the destination file. Block size is specified via cmdline argument.

Usage:
  --help                produce help message
  --if arg              set input file name
  --of arg              set output file name
  --block arg (=1024)   set block size [kB]

Sample:
  $ ./signature --if win10.iso --of signature.crc --block 1024

Dependencies:
  - boost/program_options
  - boost/crc


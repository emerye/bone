#!/usr/bin/perl
use strict; 
use warnings; 


my ($infile, $outFile, $i, $fh, $r, $g, $b, $line, $ret); 
my ($rval, $gval, $bval, $rgbout, $FOUTH); 


if (@ARGV != 2) 
{
    print "Usage: InputFileName OutputFileName\n";
    exit; 
}


$infile = $ARGV[0]; 
$outFile = $ARGV[1];



open ($fh, "<", $infile) or die "Can't open " . $infile . "\n" ; 
open ($FOUTH, ">", $outFile) or die "Can't open " . $outFile . "\n" ; 
binmode($fh); 
binmode($FOUTH); 


my $count = 0; 
while ( read($fh, $line, 3))   {
	($r, $g, $b) = unpack("CCC", $line); 
	
  
#printf("Count %d  Red %02x  Green %02x  Blue %02x\n", $count, $r, $g, $b); 
$count +=1; 

$rval = ($r & 0xF8) << 8; 
$gval = ($g & 0xFC) << 3; 
$bval = ($b & 0xF8) >> 3;  

$rgbout = $rval | $gval | $bval; 

#printf("565 %04x\n",$rgbout);  


$line = pack("S", $rgbout); 
print $FOUTH $line; 


}
close ($fh); 
close ($FOUTH); 


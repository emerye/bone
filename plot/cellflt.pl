#! /usr/bin/perl
use strict;
use warnings;

use File::Copy qw(copy);

my(@cellv,$i,$writeLine,$fltfile,$outfile,$ofh);

$fltfile = './celllog.txt';
$outfile = './pcell.txt';

#unlink $fltfile;
#copy './celllog.txt', $fltfile; 
 
my $filename = $fltfile;
open(my $fh, "< :encoding(UTF-8)", $filename)
  or die "Could not open file '$filename' $!";
 
open($ofh, "> :encoding(UTF-8)", $outfile)
  or die "Could not open file '$outfile' $!";

while (my $row = <$fh>) {
  if($row =~ m/^#/) {
    print $row;
    next;
  } 
  
  chomp $row;
  @cellv = split / /,$row;
  $writeLine = 0;
  for($i=1; $i<15; $i++) {
    if(($cellv[$i] < 3000) || ($cellv[$i] > 5000)) {
       $writeLine = 1;
    }
  }
  if($writeLine == 0) {
    print "$row\n";
  }
}
close($fh);  
close($ofh);

#!/usr/bin/perl -w
use strict; 
use warnings; 

use JSON qw( decode_json ); 

my $OUTFILE = "/root/bone/src/scripts/exttemp.txt";

my ($outFile, $ret, $wgetcmd, $fh, $completefile, @lines, $temperature,
	$wgetfile, $jsonString, $minutes, $hours); 

$OUTFILE = '/root/bone/src/scripts/cweather.json'; 

#$wgetcmd = "wget -O " . $OUTFILE . ' http://api.openweathermap.org/data/2.5/weather?q=Fremont'; 
#
$wgetcmd = 'curl -o /root/bone/src/scripts/cweather.json "http://api.openweathermap.org/data/2.5/weather?id=5350734&appid=86fd40302a1f892f8cd7528036289523"';


$ret = system($wgetcmd); 
print ("Return value:  ",  $ret, "\n"); 

if ($ret != 0) 
{
  exit(); 
}

#JSON
$jsonString = "";
open ($fh, "<", $OUTFILE) or die ("Could not open file '$OUTFILE' $!");  
while(<$fh>) {
  $jsonString = $jsonString .  $_ ; 
}
close($fh); 

#
if (-e "/root/bone/src/tft/lock.txt") 
{
  exit(); 
}

$OUTFILE = '/root/bone/src/tft/wdata.txt';
open ($fh, ">", $OUTFILE) or die ("Could not open file '$OUTFILE' $!");  
my $decoded = decode_json($jsonString); 

my $sunrise = $decoded->{'sys'}{'sunrise'};
($minutes,$hours) = (localtime($sunrise))[1,2];  
printf $fh "sunrisehour=$hours" . "\n"; 
if(length($minutes) == 1) 
{
 $minutes = "0".$minutes;  
}
printf $fh "sunriseminutes=$minutes" . "\n"; 

my $sunset = $decoded->{'sys'}{'sunset'}; 
($minutes,$hours) = (localtime($sunset))[1,2];  
printf $fh "sunsethour=" . ($hours - 12) . "\n"; 

if(length($minutes) == 1) 
{
 $minutes = "0".$minutes;  
}
printf $fh "sunsetminutes=$minutes" . "\n"; 

print $fh "message = " . $decoded->{'sys'}{'message'} . "\n";

my @weather = @{ $decoded->{'weather'} }; 
foreach my $f ( @weather )  {
  print $fh "id=" .  $f->{ 'id' } . "\n"; 
  print $fh "main=" . $f->{ 'main' } . "\n"; 
  print $fh "description=" . $f->{ 'description' } . "\n"; 
  print $fh "icon=" . $f->{ 'icon' } . "\n"; 
}

print $fh "pressure=" . $decoded->{'main'}{'pressure'} . "\n";
print $fh "sea_level=" . $decoded->{'main'}{'sea_level'} . "\n";
print $fh "grnd_level=" . $decoded->{'main'}{'grnd_level'} . "\n";
print $fh "humidity=" . $decoded->{'main'}{'humidity'} . "\n";

print $fh "windspeed=" . $decoded->{'wind'}{'speed'} . "\n";
print $fh "winddeg=" . $decoded->{'wind'}{'deg'} . "\n";

close($fh); 


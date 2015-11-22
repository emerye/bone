#!/usr/bin/perl -w

use strict;
use Gtk3 '-init'; 

my $window = Gtk3::Window->new; 
$window->set_title("This is the window title."); 
$window->set_default_geometry(500, 500); 

$window->signal_connect(destroy => sub {Gtk3->main_quit;}); 

my $label = Gtk3::Label->new ('Hello World!'); 
$window->add ($label); 
$window->show_all;
Gtk3->main; 

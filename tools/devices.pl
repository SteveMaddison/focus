#!/usr/bin/perl -w

#
# Script to test device name hashing
#

use strict;

my @devs = qw /ser par hd fd crt kb eth mou ram rom bla ccc/;
my %occur;

for my $dev ( @devs ) {
	my $sum = 0;
	my $i;
	for ( $i = 0 ; $i < length $dev ; $i++ ) {
		$sum += ord( substr( $dev, $i, 1 ) ) << $i + 1;
		print ">> ", substr( $dev, $i, 1 ), ": ", $sum, "\n";
	}

	for my $i ( 0..3 ) {
		my $hash = ( $sum + $i );
		print ">> + $i\n";
		$hash &= 0x3f;
		$occur{ $hash } ++;
		print "$dev$i : $hash\n";
	}
}

for my $key ( sort { $a <=> $b } keys %occur ) {
	print "Hash $key: $occur{ $key }\n";
}



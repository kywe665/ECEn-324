#!/usr/bin/perl
#!/usr/local/bin/perl

#
# correctness.pl - Test ncopy assembly code for correctness
#
use Getopt::Std;

#
# Configuration
#
$blocklen = 64;
$yas = "../misc/yas";
$yis = "../misc/yis";
$gendriver = "./gen-driver.pl";
$fname = "cdriver";
$verbose = 1;

#
# usage - Print the help message and terminate
#
sub usage {
    print STDERR "Usage: $0 [-hq] [-n N] -f FILE\n";
    print STDERR "   -h      Print help message\n";
    print STDERR "   -q      Quiet mode (default verbose)\n";
    print STDERR "   -n N    Set max number of elements up to 64 (default $blocklen)\n";
    print STDERR "   -f FILE Input .ys file is FILE\n";
    die "\n";
}

getopts('hqn:f:');

if ($opt_h) {
    usage();
}

if ($opt_q) {
    $verbose = 0;
}

if ($opt_n) {
    $blocklen = $opt_n;
    if ($blocklen < 0 || $blocklen > 64) {
	print STDERR "n must be between 0 and 64\n";
	die "\n";
    }
}

# Filename is required
if (!$opt_f) {
    usage();
} 
else {
    $ncopy = $opt_f;
    # Strip off .ys
    $ncopy =~ s/\.ys//;
}

if ($verbose) {
    print "\t$ncopy\n";
}

$goodcnt = 0;

for ($i = 1; $i <= $blocklen; $i++) {
	!(system "$gendriver -c -n $i -f $ncopy.ys > $fname$i.ys") ||
	die "Couldn't generate driver file $fname$i.ys\n";
	!(system "$yas $fname$i.ys") ||
	die "Couldn't assemble file $fname$i.ys\n";
	!(system "$yis $fname$i.yo > $fname$i.yis") ||
	die "Couldn't simulate file $fname$i.yo\n";
	$stat = `grep eax $fname$i.yis`;
	!(system "rm $fname$i.ys $fname$i.yo $fname$i.yis") ||
	die "Couldn't remove files $fname$i.ys and/or $fname$i.yo and/or $fname$i.yis\n";
	chomp $stat;
	$result = "failed";
	if ($stat =~ "zzzz") {
		$result = "Couldn't run checking code";
	}
	if ($stat =~ "aaaa") {
		$result = "OK";
		$goodcnt ++;
	}
	if ($stat =~ "ffff") {
		$result = "Bad count";
	}
	if ($stat =~ "eeee") {
		$result = "Incorrect copying";
	}
	if ($verbose) {
		printf "%d\t%s\n", $i, $result;
	}
}

printf "$goodcnt/$blocklen pass correctness test\n";



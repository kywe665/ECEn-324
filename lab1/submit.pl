#!/usr/bin/perl
#!/usr/local/bin/perl
use Getopt::Std;

#######################################################################
# submit.pl - Submit an entry to the Data Lab "Beat the Prof" Contest
#
# Students: Copy this script to your datalab-handout directory (the
# one with btest.c bits.c, and the dlc binary). Then type "submit.pl"
# to enter your solution in the contest and have your operator count
# displayed on the class Web page.
#
# Copyright (c) 2003, R. Bryant and D. O'Hallaron, All rights reserved.
# May not be used, modified, or copied without permission.
#
#######################################################################

#######################################################################
# egm 07-01-03
#
# Modifed this file to meets the needs of ECEn-CS 324 CS version
# of the course:
#
# * mail message includes names and ID for each team member.
# * mail message includes the bits.c file for the team on handin
#
#######################################################################

#######################################################################
# Instructors: change the following variables for your environment
# These should be *identical* to the definitions in config.pm.
#######################################################################

$LABID = "datalabwinter13";
$MAIL_USER = "ecen324";
$MAIL_HOST = "et.byu.edu";
$SENDMAIL_BIN = "/usr/sbin/sendmail";
$SENDMAIL_ARGS = "-bm";
$CONTEST_KEY = "entry";

###########################
# End of configuration info
###########################

$| = 1; # autoflush output on every print statement

# Any files created by this script are readable only by the user
umask(0077); 

#
# usage - print help message and terminate
#
sub usage {
    printf STDERR "$_[0]\n";
    printf STDERR "This script assumes that it is running in the\n";
    printf STDERR "datalab-handout directory that contains btest.c, bits.c,\n";
    printf STDERR "and the dlc binary.\n\n";
    printf STDERR "Usage: $0 [-h]\n";
    printf STDERR "Options:\n";
    printf STDERR "  -h   Print this message.\n";
    die "\n";
}

##############
# Main routine
##############

# prefix in btest output that identifies a team
$TEAMPREFIX = "Team: "; 

# +++++ egm 07/16/03
# Grab the member names and ids to use in handing in the bits.c file.
$MEMBER1PREFIX = "Member 1:\t";
$MEMBER2PREFIX = "Member 2:\t";
# -----

# 
# Parse the command line arguments
#
getopts('h');
if ($opt_h) {
    usage();
}

$infile = "bits.c";
($infile_basename = $infile) =~ s#.*/##s; # basename of input file
$cwdir = `pwd`;                  # get full pathname of current working directory
chomp($cwdir);
$tmpdir = "/tmp/submit.$$";      # temporary directory
$0 =~ s#.*/##s;                  # this prog's basename (for error msgs)

# 
# This is a message we use in several places when the program dies
#
$diemsg = "The output is in $tmpdir.";

########################################################################
# !! This functionality removed 9/7/2007 due to CS machine name changes
# Make sure they are on a CS machine when they run the submission
########################################################################
$hostname = $ENV{'HOSTNAME'};
($hostname =~ m/^*\.ee\.byu\.edu$/) or 
    die "$0: ERROR: submission failed because $hostname is not a SPICE machine";

#########################################################################
# Part I: Computer the correctness and op counts of the students solution
#########################################################################

# 
# Make sure the input file exists and is readable
#
open(INFILE, $infile) 
    or die "$0: ERROR: could not open input file $infile\n";
close(INFILE);

# 
# Make sure that dlc and btest exist and are executable
#
(-e "./dlc" and -x "./dlc")
    or  die "$0: ERROR: No executable dlc binary.\n";
(-e "./btest" and -x "./btest")
    or  die "$0: ERROR: No executable btest binary.\n";

# 
# Make sure that dlc can compile the input file
#
system("./dlc $infile > /dev/null") == 0
    or die "$0: ERROR: encountered some problem running dlc.\n";

# 
# Set up the contents of the scratch directory
#
system("mkdir $tmpdir") == 0
    or die "$0: Couldn't make scratch directory $tmpdir.\n";
system("cp $infile $tmpdir") == 0
    or die "$0: Couldn't copy file $infile to scratch directory $tmpdir.\n";
system("cp btest.c $tmpdir") == 0
    or die "$0: Couldn't copy file btest.c to scratch directory $tmpdir.\n";
system("cp decl.c $tmpdir") == 0
    or die "$0: Couldn't copy file decl.c to scratch directory $tmpdir.\n";
system("cp tests.c $tmpdir") == 0
    or die "$0: Couldn't copy file tests.c to scratch directory $tmpdir.\n";
system("cp btest.h $tmpdir") == 0
    or die "$0: Couldn't copy file btest.h to scratch directory $tmpdir.\n";
system("cp bits.h $tmpdir") == 0
    or die "$0: Couldn't copy file bits.h to scratch directory $tmpdir.\n";
system("cp Makefile $tmpdir") == 0
    or die "$0: Couldn't copy file Makefile to scratch directory $tmpdir.\n";

#
# Run dlc first to produce a zapped version of bits.c.  By "zapping"
# we mean that we remove the bodies of any functions that are
# incorrect or exceed the operator count.  
#
print "Running dlc to check for correctness\n";
system("(cd $tmpdir; $cwdir/dlc -z -o zap.c $infile)") == 0 
    or die "$0: ERROR: Zapped $infile_basename did not compile (dlc). $diemsg\n";
system("cp $tmpdir/zap.c $tmpdir/bits.c");

#
# Compile and run the zapped btest and capture the output in a scratch file
#
print "Compiling and running btest\n";
system("(cd $tmpdir; make clean > /dev/null 2>&1)");
system("(cd $tmpdir; make -s btest > /dev/null 2>&1)") == 0
    or die "$0: ERROR: $infile_basename did not compile (c). $diemsg\n";
system("$tmpdir/btest -g > $tmpdir/btest.out 2>&1") == 0
    or die "$0: ERROR: btest failed. $diemsg\n";

#
# Run dlc to get the operator count of the zapped bits.c file
#
print "Running dlc to get operator count\n";
system("(cd $tmpdir; $cwdir/dlc -W1 -e bits.c > dlc.out 2>&1)") == 0 
    or die "$0: ERROR: Zapped $infile_basename did not compile (dlc2). $diemsg\n";

#
# Read the output from btest and record the number of errors and
# name for each puzzle
#
open(BTESTFILE, "$tmpdir/btest.out") 
    or die "$0: ERROR: could not open input file $tmpdir/btest.out\n";

@puzzle_errors = (); # Number of errors reported by btest for each puzzle
@puzzle_name = ();   # Name of each puzzle
$inpuzzles = 0;      # Becomes true when we start reading puzzle results
$teamname = "none";

# +++++ egm 07/16/03
$member1name = "none";
$member2name = "none";
# -----

while ($line = <BTESTFILE>) {
    chomp($line);

    # Grab the team's name from the beginning of the btest output
    if ($line =~ /$TEAMPREFIX/) {
	($teamname = $line) =~ s/$TEAMPREFIX//;
	next;
    }
 
    # +++++ egm 07/16/03
    # Grap the member 1 name
    if ($line =~ /$MEMBER1PREFIX/) {
	($member1name = $line) =~ s/$MEMBER1PREFIX//;
	next;
    }
    # -----

    # +++++ egm 07/16/03
    # Grap the member 2 name
    if ($line =~ /$MEMBER2PREFIX/) {
	($member2name = $line) =~ s/$MEMBER2PREFIX//;
	next;
    }
    # -----

    # Notice that we're ready to read the puzzle scores
    if ($line =~ /^Score/) {
	$inpuzzles = 1;
	$puzzlecnt = 0;
	next;
    }

    # Notice that we're through reading the puzzle scores
    if ($line =~ /^Total/) {
	$inpuzzles = 0;
	next;
    }

    # Read and record a puzzle's name and score
    if ($inpuzzles) {
	($blank, $points, $errors, $name) = split(/\s+/, $line);
	$puzzle_errors[$puzzlecnt] = $errors;
	$puzzle_name[$puzzlecnt] = $name;
	$puzzlecnt++;
    }

}
close(BTESTFILE);

#
# Read the output from dlc and record the number of operators
# for each puzzle.
#
open(DLCFILE, "$tmpdir/dlc.out") 
    or die "$0: ERROR: could not open input file $tmpdir/dlc.out\n";

@puzzle_ops = ();
$puzzlecnt = 0;
while ($line = <DLCFILE>) {
    chomp($line);

    if ($line =~ /(\d+) operators/) {
	$puzzle_ops[$puzzlecnt++] = $1;
	next;
    }
}
close(DLCFILE);

###################################################
# Part II: Mail the result to the lab email address
###################################################

#
# Make sure the mail delivery program is on the system
#
(-e $SENDMAIL_BIN and -x $SENDMAIL_BIN) 
    or die "$0: ERROR: Mail program ($SENDMAIL_BIN) not found.\n";

# 
# Construct the email message
#
$mailfile = "$tmpdir/mailfile.$$";
open(MSG, ">$mailfile") 
    or die "$0: ERROR: Couldn't open $mailfile\n";
print MSG "Subject: Data Lab Notification\n\n";
for ($i = 0; $i < $puzzlecnt; $i++) {
    print MSG "$CONTEST_KEY:$LABID:$i:$teamname:";
    print MSG "$puzzle_name[$i]:$puzzle_errors[$i]:$puzzle_ops[$i]\n";
}

# +++++ egm 07/16/03
# Append the handin id with the member names at the end of the email
# message.
print MSG "\nhandin:$LABID:$teamname\n";
print MSG "$MEMBER1PREFIX$member1name\n";
print MSG "$MEMBER2PREFIX$member2name\n";
#-----

close(MSG);

# +++++ egm 07/16/03
# Append the bits.c file to the end of the email message to 
# be copied to the handin directory for each team member.
system( "cat $infile >> $mailfile" );
# -----

#
# Send the email message
#
system("$SENDMAIL_BIN $SENDMAIL_ARGS $MAIL_USER\@$MAIL_HOST < $mailfile") == 0
    or die "$0: ERROR: Couldn't run $SENDMAIL_BIN $SENDMAIL_ARGS $MAIL_USER\@$MAIL_HOST < $mailfile.\n";
print "Succesfully submitted datalab for team \"$teamname\".\n";

#
# Everything went OK, so remove the scratch directory
#

system("rm -fr $tmpdir");
exit;


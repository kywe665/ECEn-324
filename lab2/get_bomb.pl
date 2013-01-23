#! /usr/bin/perl
use Cwd;
print "This script is to help you download a bomb that is specialized for you.\nIt will ask you for some information about yourself and then it will ask you\nfor your CAEDM password. The password is used to SSH into the bomb host\nmachine and to save the bomb to your current directory. The bomb will be in \nbomb.tar in your current directory.\n\nPress ENTER to continue.";
<>;
# Get information
print "Enter your user name:";
chomp($username1 = <>);
print "Enter your full name:";
chomp($fullname1 = <>);
print "Enter your lab partners user name if you have one:";
chomp($username2 = <>);
print "Enter your lab partners full name if you have one:";
chomp($fullname2 = <>);

# Confirm information
print "\nYour user name = $username1\n";
print "Your full name = $fullname1\n";
print "Lab partners user name = $username2\n";
print "Lab partners full name = $fullname2\n";
print "\nIs this information correct?(y/n)\n";
chomp($correct = <>);
if (!($correct eq "y")) {
	print "The information is NOT correct\n";
	exit();
}

# Replace all spaces with %20
$username1 =~ s/ /\%20/;
$username2 =~ s/ /\%20/;
$fullname1 =~ s/ /\%20/;
$fullname2 =~ s/ /\%20/;

# Get bomb
my $current_dir = getcwd();
$command = "ssh $username1\@licorice.ee.byu.edu 'cd $current_dir; curl \"http://licorice.ee.byu.edu:15214/?user1mail=$username1&user1name=$fullname1&user2mail=$username2&user2name=$fullname2&submit=Submit\" >> bomb.tar'";

#print "command\n$command\n";
system($command);

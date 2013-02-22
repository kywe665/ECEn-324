#!/usr/bin/perl 
#!/usr/local/bin/perl 

#
# gen-driver - Generate driver file for any ncopy function
#
use Getopt::Std;

$n = 64;

getopts('hcn:f:');

if ($opt_h) {
    print STDERR "Usage $argv[0] [-h] [-c] [-n N] [-f FILE]\n";
    print STDERR "   -h      print help message\n";
    print STDERR "   -c      include correctness checking code\n";
    print STDERR "   -n N    set number of elements (up to 64)\n";
    print STDERR "   -f FILE set input file (default stdin)\n";
    die "\n";
}

$check = 0;
if ($opt_c) {
    $check = 1;
}


if ($opt_n) {
    $n = $opt_n;
    if ($n < 1 || $n > 64) {
	print STDERR "n must be between 1 and 64\n";
	die "\n";
    }
}

$nm1 = $n-1;

print <<PROLOGUE;
#######################################################################
# Test for copying block of size $n;
#######################################################################
	.pos 0
main:	irmovl Stack, %esp  	# Set up stack pointer
	irmovl Stack, %ebp  	# Set up base pointer

	# Set up arguments for copy function and then invoke it
	irmovl $n, %eax		# src and dst have $n elements
	pushl %eax
	irmovl dest, %eax	# dst array
	pushl %eax
	irmovl src, %eax	# src array
	pushl %eax
	call ncopy		 
PROLOGUE
if ($check) {
print <<CALL;
	call check	        # Call checker code
        halt                    # should halt with 0xaaaa in %eax
CALL
} else {
print <<HALT;
	halt			# should halt with num nonzeros in %eax
HALT
}

if ($opt_f) {
    open (CODEFILE, "$opt_f") || die "Can't open code file $opt_f\n";
    while (<CODEFILE>) {
	printf "%s", $_;
    }
} else {
    while (<>) {
	printf "%s", $_;
    }
}

if ($check) {
print <<CHECK;
   # This is the correctness checking code.  It checks that
   # %eax has $nm1, and that the destination holds values 0 through $nm1
   # The function sets %eax to 0xaaaa if all checks pass, 
   # to %0xeeee if the count is wrong, and %0xffff if the copying is incorrect
check: irmovl \$$nm1,%edx
   subl %edx,%eax
   je check2
   irmovl \$0xeeee,%eax
   jmp cdone
check2: xorl %eax,%eax  # Value being tested
   irmovl dest, %edx     # Pointer to next value to check
   irmovl \$$n,%ecx      # Counter
   andl %ecx,%ecx
   je csuccess          # done
cloop: mrmovl (%edx),%esi  # Get *dest
   subl %eax,%esi          # Subtract val
   je cok                  # If nonzero, fail
   irmovl \$0xffff, %eax   # Bad copying
   jmp cdone
cok: irmovl \$4,%esi
   addl %esi,%edx          # dest ++
   irmovl \$1,%esi
   addl %esi,%eax          # val++
   subl %esi,%ecx          # cnt--
   jg cloop
csuccess: irmovl \$0xaaaa,%eax
cdone: ret
CHECK
}

print <<EPILOGUE;
#################################################################### 
# Epilogue code for the correctness testing driver
####################################################################

###############################
# Source and destination blocks 
###############################
	.align 4
src:	.long 0
	.long 1
	.long 2
	.long 3
	.long 4
	.long 5
	.long 6
	.long 7
	.long 8
	.long 9
	.long 10
	.long 11
	.long 12
	.long 13
	.long 14
	.long 15
	.long 16
	.long 17
	.long 18
	.long 19
	.long 20
	.long 21
	.long 22
	.long 23
	.long 24
	.long 25
	.long 26
	.long 27
	.long 28
	.long 29
	.long 30
	.long 31
	.long 32
	.long 33
	.long 34
	.long 35
	.long 36
	.long 37
	.long 38
	.long 39
	.long 40
	.long 41
	.long 42
	.long 43
	.long 44
	.long 45
	.long 46
	.long 47
	.long 48
	.long 49
	.long 50
	.long 51
	.long 52
	.long 53
	.long 54
	.long 55
	.long 56
	.long 57
	.long 58
	.long 59
	.long 60
	.long 61
	.long 62
	.long 63
        .long 0x0f0f0f0f # This shouldn't get moved

	.align 16
dest:	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xbeef
	.long 0xdeadbeef

.align 4
# Run time stack
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
Stack:
EPILOGUE


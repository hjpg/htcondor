#!/usr/bin/perl
use File::Copy;
use File::Path;
use Getopt::Long;
use Digest::MD5;


GetOptions (
		'help' => \$help,
		'megs=i' => \$megs,
);

my $rows = $megs;
my $rowsz = 1048576;
my $start_char = 0;

if ( $help )    { help() and exit(0); }


my $oldmd5;
open(ORIGMD5,"<datamd5") || die "Can't open output file $!\n";
while(<ORIGMD5>)
{
	chomp($_);
	$oldmd5 = $_;
}
close(ORIGMD5);
print "Old MD5 = $oldmd5\n";

open(MD5,">backdatamd5") || die "Can't open MD5 output file $!\n";
my $datamd5 = Digest::MD5->new;
open(DATA,"<backdata") || die "Trying to open data file\n" ;
$datamd5->addfile(DATA);
close(DATA);


my $hexmd5 = $datamd5->hexdigest;

print MD5 "$hexmd5\n";
close(MD5);

if($oldmd5 eq $hexmd5)
{
	print "worked\n";
	#verbose_system("touch WORKED");
	exit(0);
}
else
{
	print "failed\n";
	#verbose_system("touch FAILED");
	exit(1);
}

# =================================
# print help
# =================================

sub help 
{
    print "Usage: writefile.pl --megs=#
Options:
	[-h/--help]				See this
	[-m/--megs]				Number of megs to make file
	\n";
}

sub verbose_system 
{

my @args = @_;
my $rc = 0xffff & system @args;

printf "system(%s) returned %#04x: ", @args, $rc;

	if ($rc == 0) 
	{
		print "ran with normal exit\n";
		return $rc;
	}
	elsif ($rc == 0xff00) 
	{
		print "command failed: $!\n";
		return $rc;
	}
	elsif (($rc & 0xff) == 0) 
	{
		$rc >>= 8;
		print "ran with non-zero exit status $rc\n";
		return $rc;
	}
	else 
	{
		print "ran with ";
		if ($rc &   0x80) 
		{
			$rc &= ~0x80;
			print "coredump from ";
			return $rc;
		}
		print "signal $rc\n"
	}
}


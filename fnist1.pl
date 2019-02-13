#!/usr/bin/perl
# $Id: fnist1.pl,v 11.5 2014/03/07 19:26:38 lyonsd Exp $
# $Log: fnist1.pl,v $
# Revision 11.5  2014/03/07 19:26:38  lyonsd
# Fe^26+ + H
#
# Revision 10.5  2014/03/06 19:47:18  lyonsd
# O^6+ + H
#
# Revision 10.0  2014/03/06 19:31:32  lyonsd
# O^6+ + He
#
# Revision 9.5  2014/03/06 18:47:51  lyonsd
# Ne^8+ + H
#
# Revision 9.1  2014/03/06 18:46:55  lyonsd
# Ne^8+ + H
#
# Revision 9.0  2014/03/06 18:22:04  lyonsd
# Ne^8+ + He
#
# Revision 8.7  2014/03/06 18:06:32  lyonsd
# Mg^9+ + H (triplets)
#
# Revision 8.4  2014/03/06 17:41:48  lyonsd
# Ne^9+ + H (singlets)
#
# Revision 8.0  2014/03/06 00:13:40  lyonsd
# Ne^9+ + He
#
# Revision 7.5  2014/03/05 23:42:45  lyonsd
# Ne^10+ + H
#
# Revision 7.0  2014/03/05 22:51:16  lyonsd
# Ne^10+ + He
#
# Revision 6.5  2014/03/05 21:29:43  lyonsd
# Mg^8+ + H
#
# Revision 6.0  2014/03/05 19:54:51  lyonsd
# Mg^8+ + He
#
# Revision 5.7  2014/03/05 19:24:46  lyonsd
# Mg^9+ + H (triplets)
#
# Revision 5.4  2014/03/05 18:39:06  lyonsd
# Mg^9+ + H (singlets)
#
# Revision 5.0  2014/03/05 00:32:33  lyonsd
# Mg^9+ + He
#
# Revision 4.6  2014/03/04 23:57:07  lyonsd
# Mg^10 + H
#
# Revision 4.5  2014/03/04 23:54:49  lyonsd
# Mg^10+ + H
#
# Revision 4.0  2014/03/04 23:32:59  lyonsd
# Mg^10+ + He
#
# Revision 3.9  2014/03/04 23:06:44  lyonsd
# Mg^11+ + H (triplets)
#
# Revision 3.8  2014/03/04 22:23:57  lyonsd
# Mg^11+ + H (singlets)
#
# Revision 3.7  2014/03/04 21:40:12  lyonsd
# Mg^11 + + He
#
# Revision 3.2  2014/03/01 09:16:16  lyonsdav
# Working version for all systems
#
# Revision 3.1  2014/03/01 05:26:50  lyonsdav
# Mg^12+ + He
#
# Revision 1.2  2014/02/28 01:13:32  lyonsdav
# O^6+ + He
#
# Revision 1.1  2014/02/21 01:22:31  lyonsdav
# Initial revision
#

#my $file="MGXI.nist";
my @line=();
my $index=0;
my %ind_file=();
my $a=undef;
my $b=undef;
my $c=undef;
my $d=undef;
my $alast=undef;
my $blast=undef;
my $clast=undef;
my $dlast=undef;
my $i=0;
my $words=0;

#open(INFILE, $file) or die($!);
while(<>)
{
	$i++;
	chomp($_);
	my $words=scalar(split(/\s+/,$_));
	my ($a,$b,$c,$d)=split(/\s+/,$_);
	if($a eq '')
	{
			$d=$c;
			$c=$b;
			$b=$blast;
			$a=$alast;
	}
	if($c eq '' && $d eq '')
	{
		$d=$b;
		$b="-";
		$c="-";
	}
	if($d eq '')
	{
		$d=$c;
		$c="-";
	}
	if($words=4)
	{
		$alast=$a;
		$blast=$b;
		$clast=$c;
		$dlast=$d;
	}
	printf("$a\t$b\t$c\t$d\n",$a,$b,$c,$d);
}

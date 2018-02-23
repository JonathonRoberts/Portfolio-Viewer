#!/usr/bin/perl
use strict;
use warnings;
use WWW::Mechanize;
use HTML::TreeBuilder::XPath;
use Email::Sender::Simple qw(sendmail);
use Email::Sender::Transport::SMTP ();
use Email::Simple ();
use Email::Simple::Creator ();

sub lsequote{
	my $url = "http://www.lse.co.uk/SharePrice.asp?shareprice=" . $_[0];

	my $mech=WWW::Mechanize->new();
	$mech->get($url);
	my $tree=HTML::TreeBuilder::XPath->new();
	$tree->parse($mech->content);

	my $content;# XPath output
	my @nodes = $tree->findnodes('/html/body/div[3]/div[1]/div[2]/div[1]/div/div[6]/div[1]/div[2]');
	foreach(@nodes){
		$content = $_->findvalue('div[2]');
		if ( $content =~/^\d*\.?\d*/i){ #check we've found a float
			return $content;
		}
	}
}
sub cryptoquote{
	my $url = "http://blockchain.info/tobtc?currency=GBP&value=1";

	my $mech=WWW::Mechanize->new();
	$mech->get($url);
	return 1/$mech->content;
}

my $wealth = 0;
my $message;
my $amount;
my $price;
my $ticker;
my $config = "config";

printf("Asset\t|\tPrice\t|\tAmount\t|\tWorth\n");

open(my $fh, "<", $config)
	or die "Can't open < $config: $!";
while(<$fh>){
	chomp;
	if(/^stock:/){
		if(/:([^:]+).l:(.+)/){
			$ticker = $1;
			$amount = $2;
			$price=&lsequote($ticker);
			printf(" %s\t |\t%.2f\t |\t%.2f\t |\t%.2f\n",$ticker,$price, $amount,$price*$amount);
			$wealth+=$price*$amount;
		}
	}
	if(/^crypto/){
		if(/:(btc):(.+)/){
			$ticker = $1;
			$amount = $2;
			$price=&cryptoquote($ticker);
			printf(" %s\t |\t%.2f\t |\t%.2f\t |\t%.2f\n",$ticker,$price, $amount,$price*$amount);
			$wealth+=$price*$amount;
		}
	}
}
printf("Net = %.2f\n",$wealth);



if(defined($message)){
	print $message;
	my $smtpserver = 'smtp.server.com';
	my $smtpport = 587;
	my $smtpuser   = 'email@server.com';
	my $smtppassword = 'password123';

	my $transport = Email::Sender::Transport::SMTP->new({
	host => $smtpserver,
	port => $smtpport,
	ssl  => 'starttls',
	port => $smtpport,
	sasl_username => $smtpuser,
	sasl_password => $smtppassword,

	});

	my $email = Email::Simple->create(
	header => [
	To      => 'recipient@server.com',
	From    => $smtpuser,
	Subject => 'Allkeyshop Offers',
	],
	body => $message,
	);

	#sendmail($email, { transport => $transport });
	print $message;
	exit 0;
}
	exit 0;


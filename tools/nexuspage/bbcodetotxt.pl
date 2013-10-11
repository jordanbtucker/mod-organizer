#!/usr/bin/perl

use Parse::BBCode::MyText;

my $p = Parse::BBCode::MyText->new();

my $code;

while (<>) {
  $code .= $_;
}

my $tree = $p->parse($code);
my $rendered = $p->render_tree($tree);
printf("%s\n", $rendered);


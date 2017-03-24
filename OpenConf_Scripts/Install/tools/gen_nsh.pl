#perl -w

use strict;
use vars qw( $tmpl $dir $flst_bat );

scalar(@ARGV) > 1 or die <<USAGE;
Usage: perl $0 path\\to\\template.nsh.pl path\\to\\source\\dir > script.nsh
USAGE

($tmpl, $dir) = @ARGV;

-f $tmpl or die "Template file not found: $tmpl!\n";

# меняем прямые на обратные (виндовые) (я вперемешку часто пишу)
$dir =~ s/\//\\/g;

# считаем, что flst.bat лежит в одном каталоге с этим скриптом
$flst_bat = $0;
$flst_bat =~ s/\//\\/g;
$flst_bat =~ s/gen_nsh\.pl$/flst.bat/i;

-f $flst_bat or die <<ERR;
flst.bat not found! 
It should be placed in the same dir as this script is ($0)
ERR

print <<HEADER;
;===========================================================================
;		СГЕНЕРИРОВАНО АВТОМАТИЧЕСКИ с помощью команды
; 			perl $0 $ARGV[0] $ARGV[1]
; 		При следующей генерации ВСЕ ИЗМЕНЕНИЯ БУДУТ ПОТЕРЯНЫ!
; 		Изменения следует вносить в шаблон $ARGV[0].
;===========================================================================

HEADER

open TMPL, "<$tmpl" or die "Can't open template file $tmpl: $!\n";

print eval join "", (<TMPL>);

close TMPL;

sub dostowin {
	my $win_chars = "\xA8\xB8\xB9\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF";
	my $dos_chars = "\xF0\xF1\xFC\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF";
	$_ = shift;
	eval("tr/$dos_chars/$win_chars/");
	return $_;
}



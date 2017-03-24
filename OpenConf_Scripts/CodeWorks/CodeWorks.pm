package CodeWorks;

#==============================================================================================================
# Модуль для массовой вставки кусков кода в модули.
# 
#==============================================================================================================
#Автор - Диркс Алексей mailto:adirks@ngs.ru
#
#Эта программа является свободным программным обеспечением. Вы можете
#распространять и (или) модифицировать ее на условиях GNU Generic Public License.
#
#Данная программа распространяется с надеждой оказаться полезной, но
#БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, в том числе без гарантий пригодности для продажи или
#каких-либо других практических целей.
#
#С полным текстом лицензии на английском языке можно ознакомитсья по адресу
#http://www.gnu.org/licenses/gpl.txt или в файле
#gnugpl.eng.txt
#
#С русским переводом лицензии можно ознакомиться по адресу
#http://gnu.org.ru/gpl.html или в файле
#gnugpl.rus.txt
#
#Вы должны получить копию GNU Generic Public License вместе с копией этой программы.
#Если это не так - сообщите об этом авторам (mailto:adirks@ngs.ru , mailto:fe@alterplast.ru) или напишите
#Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA


our @ISA = qw(Exporter);
our @EXPORT = qw(ProcessModule);

use locale;
use warnings;
use strict "vars";

#Экспортируемые переменные, в которые записываются результаты анализа модуля 
our( 
	$module_path,
	$first_proc,		#Номер первой строки самой первой процедуры в модуле
	$last_proc,			#Номер первой строки самой последней процедуры в модуле
	$last_proc_end,		#Номер последней строки самой последней процедуры в модуле
	$last_var,			#Последнее объявление переменной модуля
	$first_code,		#Первая строка кода модуля (код вне процедур/функций)
	$first_line_for_ins,#Первая строка, куда можно вставлять свою процедуру
	$last_line_for_ins, #Последняя строка, куда можно вставлять процедуру
	$proc_start,		#Первая строка искомой процедуры
	$proc_end,			#Последняя строка искомой процедуры
	$proc_body,			#Полный текст искомой процедуры
	$line_count,		#Количество строк в модуле
	@lines,				#Все строки модуля

	@errors				#Описание последней ошибки
);


my $proc_pattern = '^\s*(Процедура|Procedure|Функция|Function)\s+([\w\d]*)\s*\((.*)\)\s*(\w*).*$';
my $endproc_pattern = '^\s*(КонецПроцедуры|EndProcedure|КонецФункции|EndFunction)(\s.*)*$';
my $var_pattern = '^\s*(Перем|Var)\s+([^;]+);.*$';

sub LoadModule($)
{
    $module_path = shift;
	open MODULE, "<$module_path";
	@lines = (<MODULE>);
	close MODULE;
	foreach (@lines)
	{
		$_ =~ s/[\r\n]//g; #уберём завершающие \n и \r, если они там есть
	}
	$line_count = @lines;

	@errors = ();
}

	  
sub ProcessModule($)
{
    my $proc = shift;
	my $in_proc = 0;
	my $proc_name = "";

	$first_proc = -1;
	$last_proc = -1;
	$last_proc_end = -1;
	$last_var = -1;
	$first_code = -1;
	$first_line_for_ins = -1;
	$last_line_for_ins = -1;
	$proc_start = -1;
	$proc_end = -1;
	$proc_body = "";
	$line_count = @lines;
	my $i = 0;
	my $line;

	foreach $line (@lines)
	{
		if( $first_proc < 0 )
		{
			if( $line =~ m/$proc_pattern/i )
			{
				my $fwd = lc($4);
				if( $fwd ne "forward" and $fwd ne "далее" )
				{
					$proc_name = $2;
					$first_proc = $i;
					$last_proc = $i;
					$in_proc = 1;
				}
			}
			elsif( $line =~ m/$var_pattern/i )
			{
				$last_var = $i;
				$first_line_for_ins = $i + 1;
			}
			elsif( $line =~ m/^\s*$/ )
			{
				if( $first_line_for_ins < 0 )
				{
					$first_line_for_ins = $i + 1;
				}
			}
			else
			{
				$first_line_for_ins = 0 if ($first_line_for_ins < 0);
			}			
		}
		else
		{
			if( $line =~ m/$proc_pattern/i )
			{
				$proc_name = $2;
				$last_proc = $i;
				$in_proc = 1;
			}
			elsif( $line =~ m/$endproc_pattern/i )
			{
				if( lc($proc_name) eq lc($proc) )
				{
					$proc_end = $i;
				}
				$last_proc_end = $i;
				$last_line_for_ins = $i + 1;
				$first_code = -1;
				$in_proc = 0;
			}
			elsif( not $in_proc and $line =~ m/\S+/ )
			{
				$first_code = $i if $first_code < 0;
			}
		}

		if( $line =~ m/$proc_pattern/i and lc($2) eq lc($proc) )
		{
			$proc_start = $i;
		}

		$i++;
	}
	
	if( $proc_start >= 0 and $proc_end >= 0 )
	{
		$proc_body = join("\n", $lines[$proc_start .. $proc_end]);
	}

	$last_line_for_ins = $first_line_for_ins if $last_line_for_ins < 0;
}

sub FindProc($)
{
    my $proc = lc(shift);

	$proc_start = -1;
	$proc_end = -1;
	$proc_body = "";
	$line_count = @lines;
	my $i = 0;
	my $line;

	while( $i < $line_count )
	{
		$line = $lines[$i];
		if( $proc_start < 0 and $line =~ m/$proc_pattern/i )
		{
			my $fwd = lc($4);
			if( $fwd ne "forward" and $fwd ne "далее" and lc($2) eq $proc )
			{
				$proc_start = $i;
			}
		}
		elsif( $proc_start >= 0 and $line =~ m/$endproc_pattern/i )
		{
			$proc_end = $i;
			last;
		}

		$i++;
	}

	if( $proc_start >= 0 and $proc_end >= 0 )
	{
		$proc_body = join "\n", @lines[$proc_start .. $proc_end];
		return 1;
	}
	return 0;
}

#create procedure if it does't exist
sub CreateProc(@)
{
	my $proc_name = shift;
	my $proc_text = shift;
	my $before_proc = shift;
	my $before_proc_start = -1;
	
	if( $before_proc )
	{
		my @proc_list = split(/,/, $before_proc);
		my $i;
		for( $i = $#proc_list; $i >= 0; $i-- )
		{
			my $proc = trim($proc_list[$i]);
			if( FindProc($proc) )
			{
				$before_proc = $proc;
				$before_proc_start = $proc_start;
				last;
			}
		}
	}
		  
	ProcessModule($proc_name);
	$proc_start < 0 or return;

	my $i;
    $i = $last_line_for_ins;
    $i = SkipDownComments($last_proc_end + 1) if $last_proc_end >= 0;
	$i = $before_proc_start if $before_proc_start >= 0;
	$i = $line_count + 1 if $i < 0;
	
	#Skip upper comments
	$i = SkipUpComments($i) if $i > 0;

	my @proc_text = split(/\n/, $proc_text, -1);
	my $n = @proc_text;
	splice(@lines, $i, 0, @proc_text);

	return 1;
}

sub RemoveProc(@)
{
	my $proc_name = shift;
	my $with_comments = shift;

	if( FindProc($proc_name) )
	{
		my $i1 = $proc_start;
		my $i2 = $proc_end;
		if( $with_comments )
		{
			$i1 = SkipUpComments($i1);
			$i2 = SkipDownComments($i2);
		}
		splice(@lines, $i1, $i2 - $i1 + 1);
		return 1;
	}

	return 0;
}

sub ProcStat() #returns ($last_var, $code_start)
{
	my $i;
	my $last_var = -1;

	for( $i = $proc_start + 1; $i < $proc_end; $i++ )
	{
		$last_var = $i if $lines[$i] =~ m/^\s*(Var|Перем)\s+.*$/i;
	}
	
	my $code_start = $last_var + 1;
	$code_start = $proc_start + 1 if $last_var < 0;
	while( $code_start < $proc_end and $lines[$code_start] =~ m/^\s*$/ ) { $code_start++; } #skip blank lines

	return ($last_var, $code_start);
}
	  
#insert var declaration.
sub InsertVarDecl($$$)
{
	my $ObjName = shift; #object name for substitution. E.g.  "Контрагенты"
	my $proc_name = shift;
	my $var_name = shift;
	my $i;
	my $offset = "";
	
	ProcessModule($proc_name);

	if( $proc_name )
	{
		$proc_start >= 0 or return 0;
		for( $i = $proc_start + 1; $i < $proc_end; $i++ )
		{
			return 0 if $lines[$i] =~ m/^\s*(Var|Перем)\s+($var_name|.*?[\s,]+$var_name)[\s,;].*$/i;
		}
		my ($last_proc_var, $code_start) = ProcStat();
		if( $last_proc_var >= 0 )
			{ $i = $last_proc_var + 1; }
		else
			{ $i = $proc_start + 1; }
		$offset = "\t";
	}
	else
	{
		for( $i = 0; $i <= $last_var; $i++ )
		{
			return 0 if $lines[$i] =~ m/[\s,]+$var_name[\s,;]+/;
		}
		$i = $last_var + 1;
	}

	$var_name =~ s/<ObjectName>/$ObjName/g;
	splice(@lines, $i, 0, "$offsetПерем $var_name;");

	return 1;
}

sub RemoveVarDecl($$$)
{
	my $ObjName = shift; #object name for substitution. E.g.  "Контрагенты"
	my $ProcName = shift;
	my $VarName = shift;
	my $i;
	my $var_line = -1;
	
	$VarName =~ s/<ObjectName>/$ObjName/g;
	$ProcName =~ s/<ObjectName>/$ObjName/g;
	ProcessModule($ProcName);

	if( $ProcName )
	{
		$proc_start >= 0 or return 0;
		for( $i = $proc_start + 1; $i < $proc_end; $i++ )
		{
			if( $lines[$i] =~ m/^\s*(Var|Перем)\s+$VarName\s*;*$/i )
			{
				$var_line = $i;
				last;
			}
			elsif( not $lines[$i] =~ m/^\s*(Var|Перем)\s+.*$/i )
			{
				last;
			}
		}
	}
	else
	{
		for( $i = 0; $i <= $last_var; $i++ )
		{
			if( $lines[$i] =~ m/^\s*(Var|Перем)\s+$VarName\s*(export|экспорт)?\s*[;]*$/i )
			{
				$var_line = $i;
				last;
			}
		}
	}

	$var_line >= 0 or return 0;
	splice(@lines, $var_line, 1);

	return 1;
}

#insert module code
sub InsertCode(@)
{
	my $ObjName = shift; #object name for substitution. E.g.  "Контрагенты"
	my $proc_name = shift;
	my $code = shift;
	my $AtEnd = shift;
	my $PosPattern = shift; #Шаблон (RegExp), определяющий позицию вставки
	my ($last_proc_var, $code_start);
	my $proc_body;
	my $ins_pos;
	
	$code =~ s/<ObjectName>/$ObjName/g;
	#$code = rtrim($code);
	$proc_name =~ s/<ObjectName>/$ObjName/g;
	
	ProcessModule($proc_name);
	
	if( $proc_name )
	{
		$proc_start >= 0 && $proc_end >= 0  or return 0;
		($last_proc_var, $code_start) = ProcStat();
		my $offset = "\t";
		$ins_pos = $code_start;
		$proc_body = join("\n", @lines[$proc_start.. $proc_end]);
		
		if( $PosPattern )
		{
			my $i;
			for( $i = $proc_start; $i <= $proc_end; $i++ )
			{
				if( $lines[$i] =~ m/$PosPattern/i )
				{
					$ins_pos = $i;
					$ins_pos = $i+1 if $AtEnd;
					$lines[$i] =~ m/^(\s*).*$/;
					$offset = $1;
					last;
				}
			}
		}
		else
		{
			$ins_pos = $proc_end if $AtEnd;
		}
		$code =~ s/\n/\n$offset/gm; #добавим отступ на каждой строке
		$code = $offset . $code;
	}
	else
	{
		$proc_body = join("\n", @lines[$last_proc_end .. $#lines]);
		$ins_pos = @lines - 1;
		while( $lines[$ins_pos] =~ m/^\s*$/ ) {$ins_pos--;}
		$ins_pos++;
	}

	$proc_body =~ s/[\s\r\n]+//g;
	my $test_code = $code;
	$test_code =~ s/[\s\r\n]+//g;
	index($proc_body, $test_code) < 0 or return 0;

	my @code = split(/\n/, $code);
	splice(@lines, $ins_pos, 0, @code);

	return 1;
}

sub RenameObject($$$$)
{
	my $ObjName = shift; #object name for substitution. E.g.  "Контрагенты"
	my $ProcName = shift;
	my $OldName = shift;
	my $NewName = shift;
	my $i;

	$ProcName =~ s/<ObjectName>/$ObjName/g;
	$OldName =~ s/<ObjectName>/$ObjName/g;
	$NewName =~ s/<ObjectName>/$ObjName/g;

	ProcessModule($ProcName);
	my ($i1, $i2) = (0, $#lines);
	($i1, $i2) = ($proc_start, $proc_end) if $ProcName;
	$i1 >= 0 or return 0;

	my $changes = 0;
	for( $i = $i1; $i <= $i2; $i++ )
	{
		my $line = "";
		my $rest = $lines[$i];
		if( $rest =~ m/$OldName/i )
		{
			while( length($rest) > 0 )
			{
				#print wintodos("1: $rest   :   $line\n");
				if( $rest =~ m/^($OldName)([\s,.;()=+\-*%].*)$/i )
				{
					$line .= $NewName;
					$rest = $2;
					$changes = 1;
				}
				elsif( $rest =~ m/^(.*?[\s,;()=+\-*%]+)(.*)$/ )
				{
					$line .= $1;
					$rest = $2;
				}
				elsif( $rest =~ m/^("[^"]*")([\s,.;()=+\-*%].*)$/i )
				{
					$line .= $1;
					$rest = $2;
				}
				else
				{
					$line .= $rest;
					$rest = "";
				}
				#print wintodos("2: $rest   :   $line\n");
			}
			#print wintodos("$rest   :   $line\n");
			$lines[$i] = $line;
		}
	}

	return $changes;
}

sub ReplaceCode($$$$)
{
	my $ObjName = shift; #object name for substitution. E.g.  "Контрагенты"
	my $ProcName = shift;
	my $OldCode = shift;
	my $NewCode = shift;

	$ProcName =~ s/<ObjectName>/$ObjName/g;
	$OldCode =~ s/<ObjectName>/$ObjName/g;
	$NewCode =~ s/<ObjectName>/$ObjName/g;
	#$NewCode = rtrim($NewCode);

	ProcessModule($ProcName);
	my ($i1, $i2) = (0, $line_count - 1);
	($i1, $i2) = ($proc_start, $proc_end) if $ProcName;
	$i1 >= 0 or return 0;

	my $changes = 0;
	my $i;
	for( $i = $i1; $i <= $i2; $i++ )
	{
		if( $lines[$i] =~ m/^(\s*)(?:$OldCode)(?:\s*)$/i )
		{
			my $offset = $1;
			my $code = $NewCode;
			$code =~ s/\n/\n$offset/gm; #добавим отступ на каждой строке
			$code = $offset . $code;
			$lines[$i] =~ s/^\s*$OldCode\s*$/$code/i;
			
			$changes = 1;
			if( trim($lines[$i]) eq "" )
			{
				splice(@lines, $i, 1); #remove empty line
				$i--;
				$i2--;
			}
		}
	}

	return $changes;
}

sub RemoveCode($$$)
{
	my $ProcName = shift;
	my $StartMarker = shift;
	my $EndMarker = shift;

	my ($i1, $i2) = (0, @lines-1);

	if( $ProcName )
	{
		FindProc($ProcName) or return 0;
		($i1, $i2) = ($proc_start, $proc_end);
	}
	
	#найдём все блоки кода, и запишем их в @blocks
	my @blocks;
	my $block_start = -1;
	my $nesting_level = 0;
	my $i;
	my @matches;
	for( $i = $i1; $i <= $i2; $i++ )
	{
		if( @matches = ($lines[$i] =~ m/^\s*$StartMarker\s*$/i) )
		{
			$block_start = $i if $nesting_level == 0;
			$nesting_level++;
#			foreach(@matches) { print "'$_'\n"; }
#			print @matches, "\n";
		}
		elsif( $lines[$i] =~ m/^\s*$EndMarker\s*$/i )
		{
			$nesting_level > 0 or PushError("Warning: Стр. $i: Концевой маркер перед начальным маркером");

			$nesting_level--;
			if( $nesting_level == 0 )
			{
				push @blocks, [$block_start, $i];
			}
		}
	}

	$nesting_level == 0 or PushError("Warning: количество начальных и концевых маркеров не совпадает");

	#удаляем блоки кода, начиная с последнего
	for( $i = @blocks - 1; $i >= 0; $i-- )
	{
		splice(@lines, $blocks[$i]->[0], $blocks[$i]->[1] - $blocks[$i]->[0] + 1);
	}

	return 1;
}

sub trim($)
{
	my $str = shift;
	$str or return "";
	
	return ltrim(rtrim($str));
}

sub ltrim($)
{
	my $str = shift or return "";
	my $i = 0;
	my $len = length($str);
	while( $i < $len && index(" \t\n\r", substr($str, $i, 1)) >= 0 ) { $i++; }
	return substr($str, $i);
}

sub rtrim($)
{
	my $str = shift or return "";
	my $i = length($str) - 1;
	while( $i >= 0 && index(" \t\n\r", substr($str, $i, 1)) >= 0 ) { $i--; }
	return substr($str, 0, $i+1);
}

sub IsEmpty($)
{
	my $str = shift or return 1; # undef или "" - это сразу "пусто" (дабы избежать варнингов при матчинге)
	return $str =~ m/^[\s\r\n]*$/;
}

sub IsComment($)
{
	my $str = shift or return 0;
	return $str =~ m/^\s*\/\/.*$/;
}

#Возвращает номер строки, выше которой уже нет комментариев
sub SkipUpComments($)
{
	my $i = shift;
	while( !IsEmpty($lines[$i]) and IsComment($lines[$i-1]) ) { $i--; }
	return $i;
}

#Возвращает номер строки, ниже которой уже нет комментариев
sub SkipDownComments($)
{
	my $i = shift;
	while( !IsEmpty($lines[$i]) and IsComment($lines[$i+1]) ) { $i++; }
	if( IsComment($lines[$i]) ) { $i++; }
	return $i;
}

sub PushError($)
{
	my $err = shift;
	push @errors, $err;
	print wintodos($err), "\n";
}

sub PrintErrors()
{
	foreach( @errors )
	{
		print wintodos($_), "\n";
	}
}

sub GetErrorsDescr()
{
	my $descr = "";
	foreach( @errors )
	{
		$descr .= "$_\n";
	}
	return $descr;
}


sub GetObjectName
{
	use File::Basename;
	my $fullname = shift;

	my $dir = File::Basename::dirname( $fullname );
	my $fname = File::Basename::basename( $fullname );

	my @parts = split /[\/\\]+/, $dir;
	my $i = $#parts;
	while( $parts[$i] =~ m/\./ or lc($parts[$i]) eq "формагруппы" ) { $i--; }

	my $ObjName = $parts[$i];
	my $type = $parts[$i-1];
	my $module_type = "";
	
	
	if( lc($type) eq "документы" )
	{
		if( lc($fname) eq "модульпроведения.1s" )
			{ $module_type = "МодульДокумента"; }
		else
			{ $module_type = "ФормаДокумента"; }
	}
	elsif( lc($type) eq "справочники" )
	{
		if( $dir =~ m|^.*?[\\/]+ФормаГруппы$|i )
			{ $module_type = "ФормаГруппы"; }
		elsif( lc(substr($dir, length($dir)-4)) eq ".fls" )
			{ $module_type = "ФормаСписка"; }
		else
			{ $module_type = "ФормаЭлемента"; }
	}
	elsif( lc($type) eq "журналыдокументов" )
		{ $module_type = "ФормаЖурнала"; }
	elsif( lc($type) eq "обработки" )
		{ $module_type = "Обработка"; }
	elsif( lc($type) eq "отчеты" )
		{ $module_type = "Отчет"; }

	return ($type, $ObjName, $module_type);
}

sub wintodos {
	my $win_chars = "\xA8\xB8\xB9\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF";
	my $dos_chars = "\xF0\xF1\xFC\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF";
	$_ = shift;
	return $_ if $^O eq "cygwin";
	eval("tr/$win_chars/$dos_chars/");
	return $_;
}


1;

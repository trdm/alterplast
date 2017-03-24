$ENGINE PerlScript
$NAME CodeIns

use CodeWorks;

sub CodeIns($$$)
{
	my $Text = shift;
	my $ObjName = shift;
	my $pattern_fname = shift;

	@CodeWorks::lines = split(/\r\n/, $Text, -1);

	if( open CODE, "<$pattern_fname" )
	{
		$code = join("", (<CODE>));
		close CODE;
		eval $code;

		$Text = join("\r\n", @CodeWorks::lines);
	}
	else
	{
		Message("Нет шаблона $pattern_fname");
	}	
		  
	return $Text;
}

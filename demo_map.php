#!/usr/local/bin/php
<?
$a = array();

while( ($line = fgets(STDIN)) !== false){
	$line = strtolower(trim($line));

	if (strlen($line)){
		$x = $line[0];

		if (!$x)
			continue;

		if ($x == ' ')
			continue;
			
		$a[$x] = @$a[$x] + 1;
	}
}

foreach($a as $k => $v)
	printf("%s\t%d\n", $k, $v);

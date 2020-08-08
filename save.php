<?php
//error_reporting(E_ALL);
//var_dump($_SERVER);
date_default_timezone_set("America/New_York");

$post_data = $_POST['data'];

$replaces = array(
        'a' => 'b',
        'b' => 'c',
        'c' => 'd',
        'd' => 'e',
        'e' => 'f',
        'f' => 'g',
        'g' => 'h',
        'h' => 'i',
        'i' => 'j',
        'j' => 'k',
        'k' => 'l',
        'l' => 'm',
        'm' => 'n',
        'n' => 'o',
        'o' => 'p',
        'p' => 'q',
        'q' => 'r',
        'r' => 's',
        's' => 't',
        't' => 'u',
        'u' => 'v',
        'v' => 'w',
        'w' => 'x',
        'x' => 'y',
        'y' => 'z',
        'z' => 'a',
                    );

        //$post_data = strtr($post_data,$replaces);  //for caesar style cipher
        
$key = "cuties";
$keyLength = strlen($key);
$keyIndex = 0;

$message = str_split($post_data);

$output = '';

foreach($message as $value) // Loop through input string array
{
    $thisValueASCII = ord($value);
    
    if(($thisValueASCII >=65 && $thisValueASCII <=90) || ($thisValueASCII >=97 && $thisValueASCII <=122))
    {
        if($thisValueASCII >= 65 && $thisValueASCII <= 90) // if is uppercase
        {
            $thisValueASCIIOffset = 65;
        } else// if is lowercase
        {
            $thisValueASCIIOffset = 97;
        }

        $letter_value_corrected = $thisValueASCII - $thisValueASCIIOffset;
        $key_index_corrected = $keyIndex % $keyLength; // This is the same as fmod but I prefer this notation.

        $key_ascii_value = ord($key[$key_index_corrected]);

        if($key_ascii_value >= 65 && $key_ascii_value <= 90) // if is uppercase
        {
            $key_offset = 65;
        } else// if is lowercase
        {
            $key_offset = 97;
        }

        $final_key = $key_ascii_value - $key_offset;

        $letter_value_encrypted = ($letter_value_corrected + $final_key)%26;
        
        $output = $output . chr($letter_value_encrypted + $thisValueASCIIOffset);

        $keyIndex++;
    }
    else
    {
        $output = $output . $value;
    }
}

    $post_data = $output;
    
if (!empty($post_data)) {
    $dir = 'YOUR-SERVER-DIRECTORY/files';

    $filename = 'datastorage.txt';
    $filename2 = 'permanentdatastorage.txt';

	$fileContent = $post_data."\n";
    $fileContent2 = $post_data."\n".date("Y/m/d")." ".date("h:i:sa")." -Frances\n\n";

	$filestatus = file_put_contents("datastorage.txt",$fileContent);
    $filestatus2 = file_put_contents("permanentdatastorage.txt",$fileContent2,FILE_APPEND);
		

    echo "test";
}
?>
<?php
/** Include PHPExcel */
require_once 'Classes/PHPExcel.php';

if ($_FILES["file"]["error"] > 0)
{
	echo "Error: " . $_FILES["file"]["error"] . "<br />";
}
else
{
	echo "Upload: " . $_FILES["file"]["name"] . "<br />";
	echo "Type: " . $_FILES["file"]["type"] . "<br />";
	echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
	echo "Stored in: " . $_FILES["file"]["tmp_name"];
	
	move_uploaded_file($_FILES["file"]["tmp_name"],
		"convert/" . $_FILES["file"]["name"]);
	echo "Stored in: " . "convert/" . $_FILES["file"]["name"];
	
	$filename = "convert/" . $_FILES["file"]["name"];
	$csv_filename = str_replace('.xlsx', '.csv',$filename);
	
	//Convert!!!
	PHPExcel_Settings::setCacheStorageMethod(PHPExcel_CachedObjectStorageFactory::cache_to_discISAM);
	$objReader = PHPExcel_IOFactory::createReader('Excel2007');
	$objPHPExcel = $objReader->load($filename);
	
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'CSV');
	try{
		$objWriter->save($csv_filename);
	}catch (PHPExcel_Writer_Exception $e){
		echo 'Message :'.$e->getMessage();
	}
	
	echo "Convert Finished!<br/>";
	echo "ÎÄ¼þÏÂÔØ£º"."<a href='".$csv_filename."'>".$csv_filename."</a>";
	//unset($_FILES);
}
?>

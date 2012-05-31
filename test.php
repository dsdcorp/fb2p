<?php

  ini_set("max_execution_time", "0");
  ini_set("memory_limit", "-1");

include("xmlp.inc");
include("progress.inc");


print("loading file data... ");
//$in_fname='0000008d.fb2';
$in_fname='276604.fb2';
//$in_fname='example.fb2';
$in_fname='Lohvitskaya_Oborotni.55375.fb2';
$fdata=@file_get_contents($in_fname);
print("OK\n");

$process_config=array(
                      'struct_only'=>false,
                      'unknown_tags_processing'=>XMLP_UT_CUT,
                      'strip_comments'=>false,
                      'tags_hash'=>$GLOBALS['XMLP_FB2_elements'],
                      'tags_processing'=>array(
                          'name_first_alpha'=>false,
                          'name_len'=>20,
                          'content_len'=>512,
                          'pattern_type'=>XMLP_TPS_STRICT,
                          'include_comments'=>true,
                       ),
                     );

print("parsing struct_with_offset... ");
progress_start(1); memreport(1);
$book_with_offset=xmlp_data2struct_with_offset($fdata, $process_config);
progress_end(true);  memreport(2);
print("OK\n");
if ($book_with_offset[1]) {
  fprintf(STDERR, "XML parser errors [".$in_fname."]:old\n");
  foreach($book_with_offset[1] as $err) {
    fprintf(STDERR, $err['description'].', '.var_export($err['data'], true));
  }
}
print("saving struct_with_offset... ");
@file_put_contents('out_1_struct_with_offset.txt', var_export($book_with_offset, true));
print("OK\n");

print("compiling struct_with_offset... ");  memreport(3);
//$out_data_with_offset=xmlp_struct2data($book_with_offset[0]);
$out_data_with_offset=array(); xmlp_compile_struct($book_with_offset[0], $out_data_with_offset); $out_data_with_offset=implode('',$out_data_with_offset); //$out_data_with_offset=var_export($out_data_with_offset, true);
print("OK\n");  memreport(4);
print("saving out_with_offset... ");
@file_put_contents('out_2_data_with_offset.txt', $out_data_with_offset);
print("OK\n");

print("getting col_with_offset... ");
$col_with_offset=xmlp_struct2book($book_with_offset[0]);
print("OK\n");
print("saving col_with_offset... ");
@file_put_contents('out_3_book_with_offset.txt', var_export($col_with_offset, true));
print("OK\n");





print("parsing struct... ");
progress_start(1);  memreport(5);
$book=xmlp_data2struct($fdata, $process_config);
progress_end(true);  memreport(6);
print("OK\n");
if ($book[1]) {
  fprintf(STDERR, "xml parser errors [".$in_fname."]:\n");
  foreach($book[1] as $err) {
    fprintf(STDERR, $err['description'].', '.var_export($err['data'], true));
  }
}
print("saving struct... ");
@file_put_contents('out_1_struct.txt', var_export($book, true));
print("OK\n");

print("compiling struct... ");  memreport(7);
//$out_data=xmlp_struct2data($book[0]);
$out_data=array(); xmlp_compile_struct($book[0], $out_data); $out_data=implode('',$out_data); //$out_data=var_export($out_data, true);
print("OK\n");  memreport(8);
print("saving out... ");
@file_put_contents('out_2_data.txt', $out_data);
print("OK\n");
        
print("getting col... ");
$col=xmlp_struct2book($book[0]);
print("OK\n");
print("saving col... ");
@file_put_contents('out_3_book.txt', var_export($col, true));
print("OK\n");
exit();

$res=array();
$offset=0;
$cnt=0;
while ($book=get_tag($fdata, $offset)) {
   array_push($res, var_export($book, true));
   $offset=$book['offset']+$book['length'];
   $cnt++;
   if ($cnt>200) break;
}

@file_put_contents('test.txt', implode("\n",$res));

function memreport($pref) {
  $mu=memory_get_usage(false);
  $mu1=memory_get_usage(true);
  fprintf(STDERR, $pref." memory usage [".$mu.",".$mu1."]\n");
}

?>

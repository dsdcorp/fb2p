<?php                                                   

  ini_set("max_execution_time", "0");
  ini_set("memory_limit", "-1");


include("progress.inc");
include("xmlp.inc");
include("genres.inc");

define("FS_DL_DIRS", 0x00000001);
define("FS_DL_FILES", 0x00000002);

define("PATH_ENC_AUTO", 0x00000000);
define("PATH_ENC_1251", 0x00000001);
define("PATH_ENC_UTF8", 0x00000002);

$out_file='./out.txt';
$src_dir='./out_dir/src';
$dest_dir='./out_dir/dest';
$dest_mount_point='/mnt/ext2/';
$storagename='.zipstorage';
$libname='Библиотека';
$path_encoding=PATH_ENC_AUTO;
$compress_storage=true;
$control_genres_export=false;
$no_leased_storage=false;
$app_instead_of_flk=false;
$dbid=0;

$GLOBALS['XMLP_mem_usage_report']=false; // if true, prints memory usage report into STDERR, so in result into prc_errors.txt
$GLOBALS['process_config']=array(
                                 'struct_only'=>false,
                                 'unknown_tags_processing'=>XMLP_UT_CUT,
                                 'strip_comments'=>false,
                                 'tags_hash'=>$GLOBALS['XMLP_FB2_elements'],
                                 'fix_known_tag_types'=>true,
                                 'tags_processing'=>array(
                                     'name_first_alpha'=>false,
                                     'name_len'=>20,
                                     'content_len'=>512,
                                     'pattern_type'=>XMLP_TPS_STRICT,
                                     'include_comments'=>true,
                                  ),
                                  'export_dirs_config'=> array (
                                     'authors'=>array(
                                        'title'=>'Авторы',
                                        'inc_all_list'=>true,
                                        'tree_map'=>array(1,3),
                                     ),
                                     'sequences'=>array(
                                        'title'=>'Серии',
                                        'inc_all_list'=>true,
                                        'tree_map'=>array(1),
                                     ),
                                  ),
                                );


func_clear_log($out_file);
import_dir($src_dir);
exit(0);

function import_dir($dname) {
  global $out_file;
  $dname=fs_normal_dir($dname);
  $fl=fs_dir_list($dname, FS_DL_DIRS);
  $flen=count($fl);
  for ($fno=0;$fno<$flen;$fno++) import_dir($dname.'/'.$fl[$fno]["dirname"]);

  $fl=fs_dir_list($dname, FS_DL_FILES);
  $flen=count($fl);
//  print_log_str($out_file, var_export($fl, true));
  print_log_str($out_file, 'dir: '.$dname);
  print_log_str($out_file, 'fcount: '.$flen);
  progress_start($flen);
  progress_set_divider('%.3f');
  $fdata='';
  for ($fno=0;$fno<$flen;$fno++) {
    progress_do($fno);
    if ($fl[$fno]["is_dir"]) {
       import_dir($dname.'/'.$fl[$fno]["dirname"]);
//       import_dir($fl[$fno]["dir"].'/'.$fl[$fno]["dirname"]);
    } else if (strtolower(trim($fl[$fno]["ext"]))=='zip') {
       import_zip($fl[$fno]["dir"].'/'.$fl[$fno]["filename"], $dname);
    } else {
       $fname=$fl[$fno]["dir"].'/'.$fl[$fno]["filename"];
       $fdata=@file_get_contents($fname);
       print_log_str($out_file, $dname.'/'.$fl[$fno]["filename"].'('.filesize($fname).')');
       do_file_parse($fl[$fno]["filename"], $fdata, array());
    }
  }
  unset($fdata);
  progress_end(true);
}

function import_zip($fname, $dname='') {
  global $out_file;
  $za = new ZipArchive();
  if ($za->open($fname)!=TRUE) return false;
  $zip_ar=array();
  $flen=$za->numFiles;
  progress_start($flen);
  $ename='';
  $estat=false;
  $esize=0;
  $fdata='';
  for ($i=0; $i<$flen;$i++) {
//       if ($i%32==0)
       progress_do($i);
       $ename=$za->getNameIndex($i);
       $estat=$za->statIndex($i);
       $esize=$estat['size'];
       if ($esize>0) {
         $fdata=$za->getFromIndex($i);
         $ename=basename($ename);
         print_log_str($out_file, $dname.'/'.basename($fname).':/'.$ename.'('.$esize.')');
         do_file_parse($ename, $fdata);
       }
  }
  unset($ename);
  unset($estat);
  unset($esize);
  unset($fdata);
  progress_end(false);
  $za->close();
  unset($za);
  return true;
}

function do_file_parse($fname, $data) {
  global $out_file, $dest_dir, $dbid, $storagename, $compress_storage, $libname, $no_leased_storage, $app_instead_of_flk, $path_encoding, $dest_mount_point;
  if (!preg_match('/^(.*)\.fb2$/i', $fname, $matches)) return false;

  $pe=$path_encoding;
  if ($pe==PATH_ENC_AUTO) {
     $pe=PATH_ENC_UTF8;
     if (preg_match('/win/i',PHP_OS)) $pe=PATH_ENC_1251;
  }

  $lre_id=(int)$matches[1];
  if ($lre_id==0) $lre_id='FALSE';
//  print_log_str($out_file, 'ID: '.$lre_id);
xmlp_memreport($fname);            

     $struct=xmlp_data2struct_with_offset($data, $GLOBALS['process_config']);

xmlp_memreport($fname);
//     return false;

     if (!$struct) {
       fprintf(STDERR, "General parser error [".$fname."]:\n");
       return false;
     }

     if ($struct[1]) {
       fprintf(STDERR, "XML parser errors [".$fname."]:\n");
       foreach($struct[1] as $err) {
         fprintf(STDERR, $err['description'].', '.var_export($err['data'], true));
       }
       return false;
     }


     $book=xmlp_struct2book($struct[0]);
     if ($book==false) {
       fprintf(STDERR, "\n\n\nfile parse error: ".$fname."\n\n\n");
       return false;
     }
     if ($book) {
       $dirs=get_export_dirs($book);
xmlp_memreport($fname);
       if ($dirs && count($dirs)>0) {
         $data=xmlp_struct2data($struct[0]);
xmlp_memreport($fname);
         $dbid++;
         $fext=($compress_storage)?'.zip':'.fb2';
         $key=sprintf("%08x",$dbid);
         $subdir=sprintf("%08x",(int)($dbid/10000)).'/'.sprintf("%08x",(int)($dbid/100));
         $path=$storagename.'/'.$subdir;
         $epath=$dest_dir.'/'.$path;
         $rpath=$dest_dir.'/'.$path;
         $ezfn=$epath.'/'.$key.$fext;
         $rzfn=$rpath.'/'.$key.$fext;
  
         $lfn=$key.'.flk';
         $ldp=$dest_mount_point.$path.'/'.$key.$fext;
         if ($app_instead_of_flk) {
           $lfn=safe_name($book['title']).'.app';
           $ldp="#!/bin/sh\n".
                "/ebrmain/bin/fbreader.app ".
                $ldp."\n";
         }
         $zip_link=false;

         if ($no_leased_storage) {
           $lfn=($compress_storage)?$key.'.zip':$key.'.fb2';
           $zip_link=$compress_storage;
           $ldp=$data;
         } else {
           if ($pe==PATH_ENC_UTF8) {
             win1251_to_UTF8($epath);
             win1251_to_UTF8($ezfn);
           }

           if (!file_exists($epath)) mkdir($epath,0777,true);
    
           if ($compress_storage) {
              $zwr=write_zip($ezfn, $key.'.fb2',$data);
           } else {
              file_put_contents($epath.'/'.$key.'.fb2', $data);
           }
    
//           unset($data);
                
//           if (file_exists($ezfn)) {
//           }
         }
         $dlen=count($dirs);
         for ($i=0;$i<$dlen;$i++) {
           $lpath=$dest_dir.'/'.$libname.'/'.$dirs[$i];
           if ($pe==PATH_ENC_UTF8) win1251_to_UTF8($lpath);
           if (!file_exists($lpath)) mkdir($lpath,0777,true);
           if ($zip_link) {
              $zwr=write_zip($lpath.'/'.$lfn, $key.'.fb2', $ldp);
           } else {
              func_put_file_contents($lpath.'/'.$lfn, $ldp);
           }
         }

//         unset($book);
       }
     }

  return true;
}

function win1251_to_UTF8(&$str) {
  $src_enc='windows-1251';
  $dest_enc='UTF-8';
  $str=@iconv($src_enc, $dest_enc, $str);
}

function get_splitten_dirs($dname, $lcnt=1, $inc_all_list=false, $both_fld=false) {
  $result=array();
  $fl='';
  $fls='';
  if ($lcnt>0) {
    $fls=ucfirst(strtolower(substr($dname, 0, $lcnt)));
    $fl=$fls{0};
    if ($both_fld) $fls=$fl.'/'.$fls;
    $fls.='/';
  }
  if ($inc_all_list) array_push($result, 'Весь список/'.$dname);
  if (preg_match('/[a-z]/i',$fl)) {
      array_push($result, 'Буквы A-Z/'.$fls.$dname);
  } else if (preg_match('/[а-я]/i',$fl)) {            
      array_push($result, 'Буквы А-Я/'.$fls.$dname);
  } else {
      array_push($result, '#/'.$fls.$dname);
  }
  return $result;
}

function get_export_dirs($brec) {
  global $control_genres_export;
  $result=array();
  $res=array();
  $prefix='Серии';
  $s_dirs=array();
  $a_dirs=array();
  foreach ($brec['sequences'] as $seq) {
    $sname=safe_name($seq[0]);
    if ($sname!='') {
      $s_dirs=get_splitten_dirs($sname, 1, true);
      foreach ($s_dirs as $dir) array_push($result, $prefix.'/'.$dir);
    }
  }

  $prefix='Авторы';
  foreach ($brec['authors'] as $autor) {
    $fio=safe_name($autor[0].' '.$autor[1].' '.$autor[2]);
    if ($fio!='') {
      $a_dirs=get_splitten_dirs($fio, 3, false, true);
      foreach ($a_dirs as $dir) array_push($result, $prefix.'/'.$dir);
    }
  }

  $prefix='Жанры';
  $need_export=($control_genres_export)?false:true;
  $rcnt=count($result);
  foreach ($brec['genres'] as $genre) {
    $genre=trim($genre);
    $g=genres_get($genre);
    if (!$g) continue;
    if (!$need_export && $g[2]) $need_export=true;
    if ($control_genres_export && !$g[2]) continue;
    $meta=safe_name($g[1]);
    $gname=safe_name($g[0]);
    if ($gname=='') $gname=safe_name($genre);
    if ($gname=='') $gname='_Без жанра_';
    $path='Жанры_Авторы/'.$meta.'/'.$gname;
    foreach ($a_dirs as $dir) array_push($result, $path.'/'.$dir);
    $path='Жанры_Серии/'.$meta.'/'.$gname;
    foreach ($s_dirs as $dir) array_push($result, $path.'/'.$dir);
//    array_push($result, $path);
  }
  
  if (!$need_export) return false;

  return $result;
}




/* utility functions followed */
/* ============================================================================================================================================ */
/* ============================================================================================================================================ */
/* ============================================================================================================================================ */
/* ============================================================================================================================================ */
/* ============================================================================================================================================ */


  function fs_normal_dir($dir) {
     $dir=preg_replace("/\\\\/", '/', $dir);
     $dir=preg_replace("/[\\/]$/", '', $dir);
     return $dir;
  }

  function fs_dir_list($root, $wich=FS_DL_DIRS, $filter=false) {
     $wich = (int) $wich;
     $res = array();
     $root=fs_normal_dir($root)."/";
     if ($dir = @opendir($root)) {
        while (($file = readdir($dir)) !== false) {
           if (($wich & FS_DL_DIRS) && is_dir($root.$file) && preg_match('/[^\.]/', $file)) {
               if (!$filter || stristr($filter, $file)) {
                   $finfo=array();
                   $finfo["dirname"]=$file;
                   $finfo["dir"]=fs_normal_dir(dirname(realpath($root.$file)));
                   $finfo["is_dir"]=true;
                   $finfo["is_file"]=false;
                   array_push($res, $finfo);
               }
           }
//           print("----------------------------\n");
//           print($file.":\n");
//           print(var_export(is_dir($root.$file), true)."\n");
//           print("============================\n");
           if (($wich & FS_DL_FILES) && !is_dir($root.$file)) {
               $pi = pathinfo($file);
               if (!$filter || stristr($filter, $pi["extension"])) {
                   $finfo=array();
                   $finfo["filename"]=$pi["basename"];
                   $finfo["name"]=preg_replace("/[\\.]$/", '', substr($pi["basename"], 0, strlen($pi["basename"])-strlen($pi["extension"])));
                   $finfo["ext"]=$pi["extension"];
                   $finfo["dir"]=fs_normal_dir(dirname(realpath($root.$file)));
                   $finfo["size"]=@filesize($finfo["dir"].$finfo["filename"]);
                   $finfo["is_dir"]=false;
                   $finfo["is_file"]=true;
                   array_push($res, $finfo);
               }
           }
        }
        closedir($dir);
     }
     return $res;
  }


function print_log_str($fname, $str) {
  if (!$fname || $fname=='') return false;
  return func_print_log($fname, $str."\r\n");
}

  function func_print_log($fname, $str) {
    if (!$fname || $fname=='') return false;
    $fp = fopen ($fname, "ab");
    fputs ($fp, $str);
    fclose ($fp);
  }

  function func_clear_log($fname, $del=false) {
    if (!$fname || $fname=='') return false;
    if ($del) {
       if (file_exists($fname)) return unlink($fname);
       return false;
    }
    $fp = fopen ($fname, "w");
//    $fp = fopen ($fname, "ab");
    ftruncate($fp, 0);
    fclose ($fp);
  }

function safe_name($name) {
   $name=trim($name);
   $name=trim(preg_replace("/[^\x20A-Za-zА-Яа-яёЁ0-9\,\.\!\@\#\$\%\^\&\(\)\+\_\-\[\]\{\}\=\~\`\'\;\№\«\»]/", ' ', $name));
   $name=trim(preg_replace("/[\x20]+/", ' ', $name));
   $name=trim(preg_replace("/[\.]+$/", '', $name));
   return $name;
}

function write_zip($fname, $infname, &$fdata) {
   if (file_exists($fname)) unlink($fname);
   $zip = new ZipArchive;
   $res = $zip->open($fname, ZIPARCHIVE::CREATE);
   if ($res === TRUE) {
      $zip->addFromString($infname, $fdata);
      $zip->close();
      unset($zip);
      return true;
   }
   unset($zip);
   return false;
}

  function func_put_file_contents($filename, &$contents) {
     if (file_exists($filename)) unlink($filename);
     return file_put_contents($filename, $contents);
  }                                   



?>

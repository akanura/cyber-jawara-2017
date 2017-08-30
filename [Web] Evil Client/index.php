<?php
session_start();
include('flag.php');

if(!isset($_SESSION['USERSALT'])) {
	$_SESSION['USERSALT'] = rand(1000,9999);
	$_SESSION['PASSWORD'] = md5($_SESSION['USERSALT'].rand(1000,9999));
}

if (isset($_POST['key'])) {
	if(md5($_SESSION['USERSALT'].$_POST['key']) == $_SESSION['PASSWORD']){
		die(FLAG);
	} else {
		$_SESSION = [];
		session_destroy();
		die("wrong");
	}
}

if($_COOKIE['env'] == "development"){
	var_dump($_SERVER);
	var_dump($_SESSION);
	var_dump($_POST);
	var_dump($_GET);
	var_dump($_ENV);
}
?>

<P>UNLOCK FLAG:</P>
<form action="" method="post">
<input type=text name=key />
<input type=submit value=OK />
</form>

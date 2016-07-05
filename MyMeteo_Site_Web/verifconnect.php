<?php								/* Connection à la base de donnée */

	try
	{
		$bdd = new PDO('mysql:host=localhost;dbname=mymeteophcmeteo', 'root', '');
	}
	catch	(Exception $e)
	{
		die('Erreur :' . $e->getMessage());
	}
?>

<?php

	$pass_hache = sha1($_POST['password']);
	
$req = $bdd->prepare('SELECT Id FROM users WHERE pseudo= :pseudo AND password = :password');
$req->execute(array(
		'pseudo' => $_POST['pseudo'],
		'password' => $pass_hache));
		
$resultat = $req->fetch() ;

if (!$resultat)
{
?>



<?php
$req->closeCursor();
}


else
{

session_start() ;
$_SESSION['pseudo'] = $_POST['pseudo'];

header('Location: accueil.php');

}
$req->closeCursor();
?>

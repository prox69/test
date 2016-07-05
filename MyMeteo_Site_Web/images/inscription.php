<?php        /* Connection à la base de donnée */

 try
 {
  $bdd = new PDO('mysql:host=localhost;dbname=mymeteophcmeteo', 'root', '');
 }
 catch (Exception $e)
 {
  die('Erreur :' . $e->getMessage());
 }
?>


<!--
<?php        
 $reponse = $bdd->prepare('SELECT id FROM users WHERE pseudo = :pseudo');
 $reponse->execute(array(
   'pseudo' => $_POST['pseudo']));
   
$resultat = $reponse->fetch();
  
  if ($resultat)
  { 
?>
  
  <html>

 <head>
 
  <meta charset=utf-8 />
   
  <link rel="stylesheet" type="text/css" href="connexion.css" />
  
  <title> Intranet </title>
 
 </head>
 
 <section class="main">
 
 <p class="erreur_mdp"> Pseudo déjà utilisé. </p> <br />
 <form action="creercompte.php" method="get">
 <input type="submit" class="submit" value="Retour à la connexion"/>
 </form>
 
 </section>
 
</html>

<?php
}
 
$reponse->closeCursor();

?> -->

<?php        /* Ecriture dans la base de données */

$pass_hache = sha1($_POST['password']);  //mot de passe sécurisé

$req = $bdd->prepare('INSERT INTO users(pseudo, password, Email, societe) VALUES(:pseudo, :password, :Email, :societe)');
$req->execute(array(
  'pseudo' => $_POST['pseudo'],
  'password' => $pass_hache,
  'Email' => $_POST['email'],
  'societe' => $_POST['societe']));
  
$req->closeCursor();

   header('Location: accueil.html');

?>
<?php
session_start();
?>

<meta charset="unicode">

<html>
	<head>
		<meta charset="unicode"/>
		<title>MyMeteo - Station meteo perso</title>
		<link rel="stylesheet" type="text/css" href="style.css" />
		<meta name="author" content="BRUNET Florian - GALISSI Thomas" />
		<meta name="keywords" content="Meteo" />
		<meta name="description" content="Consultation de station météo personnelle" />
		<link rel="shortcut icon" type="image/x-icon" href="images/miniLogo.ico" />
	</head>
	
	<body>
		
		<div class="rectangleTitre1"><br />
			<a href="accueil.html"> <img src="images/logoTerre.png" alt="Logo" class="terre"/></a> <br/>
			<h1>MyMeteo - La station météo pour tous</h1>
			
			<div class="menu" align="center">
				<ul>
					<a href="accueil.php"> <li class="bouton">Accueil</li></a>
					<a href="connexion.htm"> <li class="bouton">Connexion</li></a>
					<a href="creercompte.htm"> <li class="bouton">Nouveau compte</li></a>
					<a href="cyclique.htm"> <li class="boutonActuel">Relevés</li></a>
					<a href="http://www.mymeteo.xoo.it"> <li class="bouton">Forum</li></a>
				</ul>
			</div>
		</div>
	</body>
</html>

<br/>

<table>
    <tr>
        <th>Nom du capteur </th>
        <th>Température </th>
        <th>Luminosité </th>
		<th>Pression </th>
        <th>Humidité </th>
        <th>Altitude </th>
    </tr>

<?php
error_reporting(0);
 
$doc = simplexml_load_file('http://192.168.0.11:80');

foreach($doc as $elem)
{
   
    $attributes = $elem->attributes();
    $name = $attributes["name"];
    
    $enfants = $elem->children();
    $temp = $enfants[0];
    $lumi = $enfants[1];
	$press = $enfants[2];
    $humi = $enfants[3];
    $alti = $enfants[4];
	
	$bdd = new PDO('mysql:host=localhost;dbname=mymeteophcmeteo', 'root', '');
	
	$req = $bdd->prepare('UPDATE station SET temperature = :temp, lumiere = :lumi, pression = :press, humidite = :humi, altitude = :alti');
	$req->execute(array('temp' => $temp, 'lumi' => $lumi, 'press' => $press, 'humi' => $humi, 'alti' => $alti));
	
    //$bdd->exec('INSERT INTO station(lumiere, temperature) VALUES(\'10\', \'10\')');
	//$bdd->exec('DELETE FROM station');
	//<meta http-equiv="refresh" content="5; url="cyclique.php" />
    echo "<tr><td>".$name."</td><td>".$temp."</td><td>".$lumi."</td><td>".$press."</td><td>".$humi."</td><td>".$alti."</td></tr>";
}
?>
<meta http-equiv="refresh" content="10; url="cyclique.php" />
</table>
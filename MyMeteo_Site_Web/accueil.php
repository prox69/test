<?php
session_start();
if (isset($_SESSION['pseudo'])) { // Si le membre est connecté
		echo $_SESSION['pseudo'];
	}
?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8"/>
		<title>MyMeteo - Station meteo perso ooooo</title>
		<link rel="stylesheet" type="text/css" href="style.css" />
		<meta name="author" content="BRUNET Florian - GALISSI Thomas" />
		<meta name="keywords" content="Meteo" />
		<meta name="description" content="Consultation de station météo personnelle" />
		<link rel="shortcut icon" type="image/x-icon" href="images/miniLogo.ico" />
	</head>
	
	<body>
		<div class="rectangleTitre1"><br />
			<div class="enTete">
				<a href="accueil.html"> <img src="images/logoTerre.png" alt="Logo" class="terre"/></a> <br/>
				<h1>MyMeteo - La station météo connectée</h1>


			</div>
			
			<div class="menu" align="center">
				<ul>
					<a> <li class="boutonActuel">Accueil</li></a>
					
					<?php 
					if($_SESSION == null)
						echo "<a href='connexion.php'> <li class='bouton'>Connexion</li></a>";
					else
						echo "<a href='deconnect.php'> <li class='bouton'>Deconnexion</li></a>";
					?>
					
					<a href="creercompte.htm"> <li class="bouton">Nouveau compte</li></a>
					<a href="cyclique.php"> <li class="bouton">Relevés</li></a>
					<a href="http://www.mymeteo.xoo.it"> <li class="bouton">Forum</li></a>
				</ul>
			</div>
			
		</div>
		<div class="rectangleTitre2">
			<h2><center>Bienvenue sur MyMeteo !</center></h2>
		</div>
		<div class="zoneTexte">
			<p><strong>MyMeteo n'est pas un site internet ordinaire !</strong> Ici vous pourrez :</p>
			<p><ul>
				<li>Consulter en <strong>temps réel</strong> les relevés de votre station météo personnelle où que vous soyez (moyennant une connexion à internet),</li>
				<li>Devenir <strong>membre PREMIUM</strong> pour supporter le projet et avoir accès à des fonctionnalités additionnelles,</li>
				<li>Partager vos relevés avec <a href="http://www.meteofrance.com/accueil"/> METEO FRANCE</a> ou d'autres organismes en charge d'étudier les comportements de notre planète afin d'étoffer les bases de données servant à faire des prévisions et des rétrospectives plus précises.</li></p><br />
			</ul>
			<p>En plus de ce site Web, il y a également l'<strong>interface de MyMeteo</strong> qui permet d'avoir accès à des relevés plus fréquents et à différents paramètres inédits, pour que chaque station vous corresponde et soit unique en son genre !</p>
			<p>Elle sera mise en place quand le projet prendra de l'ampleur.
			<br />
			<p>Pour plus d'informations, vous pouvez contacter les personnes en charge de ce projet :
			<p>Contacter <a href ="mailto:florian.brunet@outlook.fr">Florian BRUNET</a></p>
			<p>Contacter <a href ="mailto:galissi.thomas@gmail.com">Thomas GALISSI</a></p>
		</div>
		<br />
	</body>
</html>
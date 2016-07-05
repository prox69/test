-- phpMyAdmin SQL Dump
-- version 4.0.4
-- http://www.phpmyadmin.net
--
-- Client: localhost
-- Généré le: Lun 25 Août 2014 à 08:33
-- Version du serveur: 5.6.12-log
-- Version de PHP: 5.4.16

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de données: `mymeteo`
--
CREATE DATABASE IF NOT EXISTS `mymeteo` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `mymeteo`;

-- --------------------------------------------------------

--
-- Structure de la table `station`
--

CREATE TABLE IF NOT EXISTS `station` (
  `temperature` float NOT NULL,
  `lumiere` int(11) NOT NULL,
  `pression` int(11) NOT NULL,
  `humidite` int(11) NOT NULL,
  `altitude` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Contenu de la table `station`
--

INSERT INTO `station` (`temperature`, `lumiere`, `pression`, `humidite`, `altitude`) VALUES
(27, 825, 996, 42, 143),
(27, 825, 996, 42, 143);

-- --------------------------------------------------------

--
-- Structure de la table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `pseudo` varchar(100) NOT NULL,
  `password` varchar(100) NOT NULL,
  `Email` varchar(255) NOT NULL,
  `societe` varchar(255) NOT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Contenu de la table `users`
--

INSERT INTO `users` (`Id`, `pseudo`, `password`, `Email`, `societe`) VALUES
(1, 'thomas', '73d1ffade29fd93670a8ab7c36ef3ba9345fe026', 'sqdqsd', 'dqdqd');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

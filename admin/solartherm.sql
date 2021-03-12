-- phpMyAdmin SQL Dump
-- version 5.0.4
-- https://www.phpmyadmin.net/

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";

--
-- Datenbank: `solartherm`
--
CREATE DATABASE IF NOT EXISTS `solartherm` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `solartherm`;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `sensors`
--

CREATE TABLE `sensors` (
  `id` int(6) UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `name` varchar(50) DEFAULT NULL,
  `temperature` varchar(30) DEFAULT NULL,
  `pressure` varchar(30) DEFAULT NULL,
  `altitude` varchar(30) DEFAULT NULL,
  `humidity` varchar(30) DEFAULT NULL,
  `battery` varchar(30) DEFAULT NULL,
  `Date` date DEFAULT NULL,
  `Time` time DEFAULT NULL,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

COMMIT;

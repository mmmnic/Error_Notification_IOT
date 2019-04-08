-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Apr 08, 2019 at 12:04 PM
-- Server version: 10.1.38-MariaDB
-- PHP Version: 7.2.16

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `it_solution`
--

-- --------------------------------------------------------

--
-- Table structure for table `environment`
--

CREATE TABLE `environment` (
  `ID` int(11) NOT NULL,
  `NODEID` varchar(100) COLLATE utf8_vietnamese_ci NOT NULL,
  `TEMPERATURE` float NOT NULL,
  `HUMIDITY` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_vietnamese_ci;

--
-- Dumping data for table `environment`
--

INSERT INTO `environment` (`ID`, `NODEID`, `TEMPERATURE`, `HUMIDITY`) VALUES
(1, 'NODE1', 27.9, 48),
(2, 'NODE2', 27.5, 52);

-- --------------------------------------------------------

--
-- Table structure for table `error_noti`
--

CREATE TABLE `error_noti` (
  `id` int(11) NOT NULL,
  `History_time` datetime NOT NULL,
  `MACHINE_INDEX` int(11) NOT NULL,
  `ERROR_INDEX` int(11) NOT NULL,
  `MACHINE_ID` varchar(100) COLLATE utf8_vietnamese_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_vietnamese_ci;

--
-- Dumping data for table `error_noti`
--

INSERT INTO `error_noti` (`id`, `History_time`, `MACHINE_INDEX`, `ERROR_INDEX`, `MACHINE_ID`) VALUES
(1, '2019-04-08 16:34:45', 2, 1, 'NODE1');

-- --------------------------------------------------------

--
-- Table structure for table `node`
--

CREATE TABLE `node` (
  `ID` int(11) NOT NULL,
  `NODEID` varchar(100) COLLATE utf8_vietnamese_ci NOT NULL,
  `NODENAME` varchar(100) COLLATE utf8_vietnamese_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_vietnamese_ci;

--
-- Dumping data for table `node`
--

INSERT INTO `node` (`ID`, `NODEID`, `NODENAME`) VALUES
(1, 'NODE1', 'NODE_ABC'),
(2, 'NODE2', 'NODE_BCD');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `environment`
--
ALTER TABLE `environment`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `error_noti`
--
ALTER TABLE `error_noti`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `node`
--
ALTER TABLE `node`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `environment`
--
ALTER TABLE `environment`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT for table `error_noti`
--
ALTER TABLE `error_noti`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `node`
--
ALTER TABLE `node`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

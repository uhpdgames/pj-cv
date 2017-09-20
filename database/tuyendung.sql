CREATE TABLE `thongtin_congty` (
  `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `tencongty` varchar(50) DEFAULT NULL,
  `diachi` varchar(50) DEFAULT NULL,
  `sdt` varchar(11) DEFAULT NULL,
  `email` varchar(50) DEFAULT NULL,
  `website` varchar(50) DEFAULT NULL,
  `quymo` varchar(500) DEFAULT NULL,
  `fid_logo` int(11) DEFAULT NULL,
  `uid` varchar(255) DEFAULT NULL,
  `created` varchar(50) NOT NULL
);

CREATE TABLE `tintuyendung` (
  `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `id_cty` int(11) NOT NULL,
  `tieude` varchar(50) DEFAULT NULL,
  `nganh` varchar(50) DEFAULT NULL,
  `noilamviec` varchar(50) DEFAULT NULL,
  `soluong` int(11) NOT NULL,
  `capbac` varchar(50) DEFAULT NULL,
  `tglamviec` varchar(50) DEFAULT NULL,
  `gioitinh` int(1) NOT NULL,
  `kinhnghiem` varchar(50) DEFAULT NULL,
  `luong` varchar(50) DEFAULT NULL,
  `created` varchar(50) NOT NULL
);

CREATE TABLE `baotintuyendung` (
  `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `id_cv` int(11) NOT NULL,
  `uid` INT NOT NULL,
  `het_han` varchar(50) DEFAULT NULL,
  `email` varchar(50) DEFAULT NULL,
  `sdt` varchar(11) DEFAULT NULL,
  `mota_cty` varchar(255) DEFAULT NULL,
  `mota_congviec` varchar(255) DEFAULT NULL,
  `mota_ungvien` varchar(255) DEFAULT NULL,
  `mota_quyenloi` varchar(255) DEFAULT NULL,
  `kynang_can` varchar(255) DEFAULT NULL
);
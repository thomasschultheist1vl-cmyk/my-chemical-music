-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 15-07-2026 a las 03:57:19
-- Versión del servidor: 10.4.32-MariaDB
-- Versión de PHP: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `my_chemical_music`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `categorias`
--

CREATE TABLE `categorias` (
  `id_categoria` int(11) NOT NULL,
  `nombre` varchar(60) NOT NULL,
  `descripcion` varchar(150) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `categorias`
--

INSERT INTO `categorias` (`id_categoria`, `nombre`, `descripcion`) VALUES
(1, 'Guitarras', 'Instrumentos de cuerda'),
(3, 'Cuerdas guitarra', 'Cuerdas de guitarra'),
(4, 'Baterias', 'Baterias electricas/acusticas'),
(5, 'Bajos', 'Bajo de 4 cuerdas'),
(6, 'Cuerdas de bajo', 'Cuerdas para bajo'),
(7, 'Amplificadores de guitarra', 'Amplificadores para guitarras electricas'),
(8, 'Amplificadores para bajos', 'Amplificadores para bajos electricos'),
(9, 'Cable para guitarra/bajo', 'Cable para guitarra/bajo electrico');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `clientes`
--

CREATE TABLE `clientes` (
  `id_cliente` int(11) NOT NULL,
  `nombre` varchar(50) NOT NULL,
  `apellido` varchar(50) NOT NULL,
  `telefono` varchar(30) DEFAULT NULL,
  `email` varchar(80) DEFAULT NULL,
  `direccion` varchar(120) DEFAULT NULL,
  `dni` varchar(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `clientes`
--

INSERT INTO `clientes` (`id_cliente`, `nombre`, `apellido`, `telefono`, `email`, `direccion`, `dni`) VALUES
(1, 'tuntung', 'sahur', '6767', 'ttt@gmail.com', 'tt67', '676767'),
(2, 'Paulo', 'Londrardo', '11344223', 'adanyeva@gmail.com', 'pavarotti 456', '40225523'),
(3, 'marki', 'locuras', '12345', 'marki@gmail.com', 'la casa de marki 12', '4567683'),
(4, 'Mateo', 'Lugo', '1122564323', 'mateolugo@gmail.com', 'Ruben Juarez 5990', '457776552'),
(5, 'Matt', 'Shadows', '12367767', 'matt@gmail.com', 'Av. Sevenfold 10', '4123918'),
(6, 'Juan', 'pepe', '123888', 'pepe@gmail.com', 'lacasadepepe 1234', '423784'),
(7, 'Marcos', 'Papu', '676712', 'papu@gmail.com', 'lacasa del papu 67', '9834729');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `compras_proveedores`
--

CREATE TABLE `compras_proveedores` (
  `id_compra` int(11) NOT NULL,
  `id_proveedor` int(11) NOT NULL,
  `id_usuario` int(11) DEFAULT NULL,
  `fecha` date NOT NULL,
  `total` decimal(10,2) NOT NULL,
  `estado` varchar(20) NOT NULL DEFAULT 'Confirmada',
  `motivo_anulacion` varchar(255) DEFAULT NULL,
  `fecha_anulacion` datetime DEFAULT NULL,
  `id_usuario_anulacion` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `compras_proveedores`
--

INSERT INTO `compras_proveedores` (`id_compra`, `id_proveedor`, `id_usuario`, `fecha`, `total`, `estado`, `motivo_anulacion`, `fecha_anulacion`, `id_usuario_anulacion`) VALUES
(1, 1, NULL, '2026-09-12', 30000.00, 'Confirmada', NULL, NULL, NULL),
(4, 1, NULL, '2026-07-12', 340200.00, 'Confirmada', NULL, NULL, NULL),
(6, 1, NULL, '2026-07-09', 170100.00, 'Anulada', 'Cancelacion de compra', '2026-07-14 19:23:53', NULL),
(7, 1, 3, '2026-09-12', 217500.00, 'Confirmada', NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `detalle_compras`
--

CREATE TABLE `detalle_compras` (
  `id_detalle_compra` int(11) NOT NULL,
  `id_compra` int(11) NOT NULL,
  `id_producto` int(11) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `precio_compra` decimal(10,2) NOT NULL,
  `subtotal` decimal(10,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `detalle_compras`
--

INSERT INTO `detalle_compras` (`id_detalle_compra`, `id_compra`, `id_producto`, `cantidad`, `precio_compra`, `subtotal`) VALUES
(1, 1, 1, 1, 30000.00, 30000.00),
(2, 4, 1, 6, 56700.00, 340200.00),
(3, 6, 7, 3, 56700.00, 170100.00),
(4, 7, 7, 5, 36000.00, 180000.00),
(5, 7, 6, 3, 12500.00, 37500.00);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `detalle_ventas`
--

CREATE TABLE `detalle_ventas` (
  `id_detalle_venta` int(11) NOT NULL,
  `id_venta` int(11) NOT NULL,
  `id_producto` int(11) NOT NULL,
  `cantidad` int(11) NOT NULL,
  `precio_unitario` decimal(10,2) NOT NULL,
  `subtotal` decimal(10,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `detalle_ventas`
--

INSERT INTO `detalle_ventas` (`id_detalle_venta`, `id_venta`, `id_producto`, `cantidad`, `precio_unitario`, `subtotal`) VALUES
(1, 1, 1, 1, 350000.00, 350000.00),
(2, 2, 6, 1, 6700.00, 6700.00),
(3, 2, 8, 1, 16700.00, 16700.00),
(4, 3, 4, 1, 12000.00, 12000.00),
(5, 3, 5, 1, 30000.00, 30000.00),
(6, 4, 7, 1, 54000.00, 54000.00),
(7, 5, 6, 1, 6700.00, 6700.00);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `estados_servicio`
--

CREATE TABLE `estados_servicio` (
  `id_estado_servicio` int(11) NOT NULL,
  `nombre` varchar(40) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `estados_servicio`
--

INSERT INTO `estados_servicio` (`id_estado_servicio`, `nombre`) VALUES
(1, 'Pendiente'),
(2, 'En reparacion'),
(3, 'Listo para retirar'),
(4, 'Entregado'),
(5, 'Anulado');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `facturas`
--

CREATE TABLE `facturas` (
  `id_factura` int(11) NOT NULL,
  `id_venta` int(11) DEFAULT NULL,
  `id_servicio` int(11) DEFAULT NULL,
  `id_usuario` int(11) DEFAULT NULL,
  `numero_factura` varchar(30) NOT NULL,
  `tipo_factura` varchar(10) NOT NULL,
  `fecha` date NOT NULL,
  `total` decimal(10,2) NOT NULL,
  `estado` varchar(20) NOT NULL DEFAULT 'Emitida',
  `estado_pago` varchar(20) NOT NULL DEFAULT 'Pendiente',
  `fecha_pago` datetime DEFAULT NULL,
  `id_usuario_pago` int(11) DEFAULT NULL,
  `motivo_anulacion` varchar(255) DEFAULT NULL,
  `fecha_anulacion` datetime DEFAULT NULL,
  `id_usuario_anulacion` int(11) DEFAULT NULL
) ;

--
-- Volcado de datos para la tabla `facturas`
--

INSERT INTO `facturas` (`id_factura`, `id_venta`, `id_servicio`, `id_usuario`, `numero_factura`, `tipo_factura`, `fecha`, `total`, `estado`, `estado_pago`, `fecha_pago`, `id_usuario_pago`, `motivo_anulacion`, `fecha_anulacion`, `id_usuario_anulacion`) VALUES
(1, 1, NULL, NULL, 'F0001', 'B', '2026-06-12', 350000.00, 'Emitida', 'Pendiente', NULL, NULL, NULL, NULL, NULL),
(2, NULL, 2, NULL, '123124', 'B', '2026-07-06', 12900.00, 'Emitida', 'Pendiente', NULL, NULL, NULL, NULL, NULL),
(3, 2, NULL, NULL, '8159783', 'A', '2026-09-12', 23400.00, 'Emitida', 'Pendiente', NULL, NULL, NULL, NULL, NULL),
(4, NULL, 1, NULL, '948752', 'A', '2026-09-16', 9000.00, 'Emitida', 'Pendiente', NULL, NULL, NULL, NULL, NULL),
(5, 3, NULL, NULL, '8932865', 'B', '2026-09-19', 42000.00, 'Emitida', 'Pendiente', NULL, NULL, NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `marcas`
--

CREATE TABLE `marcas` (
  `id_marca` int(11) NOT NULL,
  `nombre` varchar(60) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `marcas`
--

INSERT INTO `marcas` (`id_marca`, `nombre`) VALUES
(1, 'Fender'),
(3, 'Pearl'),
(4, 'Earnie ball'),
(5, 'Gibson'),
(6, 'DW'),
(7, 'Marshall');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `medios_pago`
--

CREATE TABLE `medios_pago` (
  `id_medio_pago` int(11) NOT NULL,
  `nombre` varchar(40) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `medios_pago`
--

INSERT INTO `medios_pago` (`id_medio_pago`, `nombre`) VALUES
(1, 'Efectivo'),
(2, 'Transferencia');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `productos`
--

CREATE TABLE `productos` (
  `id_producto` int(11) NOT NULL,
  `nombre` varchar(80) NOT NULL,
  `descripcion` varchar(200) DEFAULT NULL,
  `precio_venta` decimal(10,2) NOT NULL,
  `stock` int(11) NOT NULL,
  `stock_minimo` int(11) DEFAULT 0,
  `id_categoria` int(11) NOT NULL,
  `id_marca` int(11) NOT NULL,
  `id_proveedor` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `productos`
--

INSERT INTO `productos` (`id_producto`, `nombre`, `descripcion`, `precio_venta`, `stock`, `stock_minimo`, `id_categoria`, `id_marca`, `id_proveedor`) VALUES
(1, 'Guitarra Stratocaster', 'Guitarra electrica color negro', 350000.00, 11, 1, 1, 1, 1),
(3, 'Cuerdas', 'Cuerdas guitarra', 5900.00, 67, 1, 3, 4, 2),
(4, 'Guitarra Jacob\'s', 'Guitarra electrica 24 cuerdas mandolina', 12000.00, 1, 1, 1, 1, 1),
(5, 'Cajon peruano', 'Cajon que sirve para percusion.', 30000.00, 6, 1, 4, 6, 1),
(6, 'Bajo', 'Bajo electrico 6 cuerdas', 6700.00, 7, 1, 5, 1, 1),
(7, 'Bateria', 'Bateria acustica 4 cuerpos', 54000.00, 5, 1, 4, 6, 1),
(8, 'Cuerdas para bajo (6)', 'Cuerdas para bajo de 6', 16700.00, 4, 1, 6, 4, 1);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `proveedores`
--

CREATE TABLE `proveedores` (
  `id_proveedor` int(11) NOT NULL,
  `nombre` varchar(80) NOT NULL,
  `telefono` varchar(30) DEFAULT NULL,
  `email` varchar(80) DEFAULT NULL,
  `direccion` varchar(120) DEFAULT NULL,
  `cuit` varchar(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `proveedores`
--

INSERT INTO `proveedores` (`id_proveedor`, `nombre`, `telefono`, `email`, `direccion`, `cuit`) VALUES
(1, 'Proveedor Musical', '1122334455', 'proveedor@email.com', 'Av. Siempre Viva 123', '20-12345678-9'),
(2, 'prov1', '67', '67@gmail.com', '67prov1', '6767');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `roles`
--

CREATE TABLE `roles` (
  `id_rol` int(11) NOT NULL,
  `nombre` varchar(40) NOT NULL,
  `descripcion` varchar(150) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `roles`
--

INSERT INTO `roles` (`id_rol`, `nombre`, `descripcion`) VALUES
(1, 'Supervisor', 'Acceso completo al sistema'),
(2, 'Vendedor', 'Ventas, servicios, clientes, facturacion y consulta de productos'),
(3, 'Encargado de compras', 'Compras a proveedores, productos, stock y proveedores');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `servicios`
--

CREATE TABLE `servicios` (
  `id_servicio` int(11) NOT NULL,
  `id_cliente` int(11) NOT NULL,
  `instrumento` varchar(80) NOT NULL,
  `descripcion` varchar(250) NOT NULL,
  `fecha_ingreso` date NOT NULL,
  `fecha_entrega` date DEFAULT NULL,
  `precio` decimal(10,2) DEFAULT NULL,
  `id_estado_servicio` int(11) NOT NULL,
  `id_usuario` int(11) DEFAULT NULL,
  `motivo_anulacion` varchar(255) DEFAULT NULL,
  `fecha_anulacion` datetime DEFAULT NULL,
  `id_usuario_anulacion` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `servicios`
--

INSERT INTO `servicios` (`id_servicio`, `id_cliente`, `instrumento`, `descripcion`, `fecha_ingreso`, `fecha_entrega`, `precio`, `id_estado_servicio`, `id_usuario`, `motivo_anulacion`, `fecha_anulacion`, `id_usuario_anulacion`) VALUES
(1, 1, 'guitarra', 'calibracion', '2026-09-09', '2026-09-15', 9000.00, 2, NULL, NULL, NULL, NULL),
(2, 1, 'guitarra 2', 'calibrar 2da guitarra', '2026-08-22', '2026-09-12', 12900.00, 4, NULL, NULL, NULL, NULL),
(3, 2, 'Guitarra', 'Arreglo de mastil (roto)', '2026-09-13', '2026-09-19', 26000.00, 5, NULL, 'Cancelo servicio el cliente', '2026-07-14 19:35:50', NULL);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `usuarios`
--

CREATE TABLE `usuarios` (
  `id_usuario` int(11) NOT NULL,
  `id_rol` int(11) NOT NULL,
  `nombre` varchar(50) NOT NULL,
  `apellido` varchar(50) NOT NULL,
  `nombre_usuario` varchar(50) NOT NULL,
  `password_hash` varchar(255) NOT NULL,
  `activo` tinyint(1) NOT NULL DEFAULT 1,
  `fecha_creacion` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `usuarios`
--

INSERT INTO `usuarios` (`id_usuario`, `id_rol`, `nombre`, `apellido`, `nombre_usuario`, `password_hash`, `activo`, `fecha_creacion`) VALUES
(1, 1, 'Thomas', 'Schultheis', 'supervisor', '03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4', 1, '2026-07-14 21:52:56'),
(2, 2, 'Camila', 'Schultheis', 'Cami', '03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4', 1, '2026-07-14 22:44:33'),
(3, 3, 'Jacksito', 'Oliver', 'Jack', '03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4', 1, '2026-07-14 22:45:40');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `ventas`
--

CREATE TABLE `ventas` (
  `id_venta` int(11) NOT NULL,
  `id_cliente` int(11) NOT NULL,
  `fecha` date NOT NULL,
  `total` decimal(10,2) NOT NULL,
  `id_medio_pago` int(11) NOT NULL,
  `id_usuario` int(11) DEFAULT NULL,
  `estado` varchar(20) NOT NULL DEFAULT 'Confirmada',
  `motivo_anulacion` varchar(255) DEFAULT NULL,
  `fecha_anulacion` datetime DEFAULT NULL,
  `id_usuario_anulacion` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `ventas`
--

INSERT INTO `ventas` (`id_venta`, `id_cliente`, `fecha`, `total`, `id_medio_pago`, `id_usuario`, `estado`, `motivo_anulacion`, `fecha_anulacion`, `id_usuario_anulacion`) VALUES
(1, 1, '2026-06-12', 350000.00, 2, NULL, 'Confirmada', NULL, NULL, NULL),
(2, 3, '2026-07-17', 23400.00, 2, NULL, 'Confirmada', NULL, NULL, NULL),
(3, 4, '2026-08-18', 42000.00, 1, NULL, 'Confirmada', NULL, NULL, NULL),
(4, 2, '2026-06-19', 54000.00, 1, NULL, 'Confirmada', NULL, NULL, NULL),
(5, 2, '2026-09-16', 6700.00, 1, NULL, 'Anulada', 'Venta cancelada', '2026-07-14 19:20:24', NULL);

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `categorias`
--
ALTER TABLE `categorias`
  ADD PRIMARY KEY (`id_categoria`);

--
-- Indices de la tabla `clientes`
--
ALTER TABLE `clientes`
  ADD PRIMARY KEY (`id_cliente`);

--
-- Indices de la tabla `compras_proveedores`
--
ALTER TABLE `compras_proveedores`
  ADD PRIMARY KEY (`id_compra`),
  ADD KEY `id_proveedor` (`id_proveedor`),
  ADD KEY `idx_compras_id_usuario` (`id_usuario`),
  ADD KEY `idx_compras_id_usuario_anulacion` (`id_usuario_anulacion`);

--
-- Indices de la tabla `detalle_compras`
--
ALTER TABLE `detalle_compras`
  ADD PRIMARY KEY (`id_detalle_compra`),
  ADD KEY `id_compra` (`id_compra`),
  ADD KEY `id_producto` (`id_producto`);

--
-- Indices de la tabla `detalle_ventas`
--
ALTER TABLE `detalle_ventas`
  ADD PRIMARY KEY (`id_detalle_venta`),
  ADD KEY `id_venta` (`id_venta`),
  ADD KEY `id_producto` (`id_producto`);

--
-- Indices de la tabla `estados_servicio`
--
ALTER TABLE `estados_servicio`
  ADD PRIMARY KEY (`id_estado_servicio`);

--
-- Indices de la tabla `facturas`
--
ALTER TABLE `facturas`
  ADD PRIMARY KEY (`id_factura`),
  ADD UNIQUE KEY `id_venta` (`id_venta`),
  ADD UNIQUE KEY `id_servicio` (`id_servicio`),
  ADD KEY `idx_facturas_id_usuario` (`id_usuario`),
  ADD KEY `idx_facturas_id_usuario_anulacion` (`id_usuario_anulacion`),
  ADD KEY `idx_facturas_estado_pago` (`estado_pago`),
  ADD KEY `idx_facturas_id_usuario_pago` (`id_usuario_pago`);

--
-- Indices de la tabla `marcas`
--
ALTER TABLE `marcas`
  ADD PRIMARY KEY (`id_marca`);

--
-- Indices de la tabla `medios_pago`
--
ALTER TABLE `medios_pago`
  ADD PRIMARY KEY (`id_medio_pago`);

--
-- Indices de la tabla `productos`
--
ALTER TABLE `productos`
  ADD PRIMARY KEY (`id_producto`),
  ADD KEY `id_categoria` (`id_categoria`),
  ADD KEY `id_marca` (`id_marca`),
  ADD KEY `id_proveedor` (`id_proveedor`);

--
-- Indices de la tabla `proveedores`
--
ALTER TABLE `proveedores`
  ADD PRIMARY KEY (`id_proveedor`);

--
-- Indices de la tabla `roles`
--
ALTER TABLE `roles`
  ADD PRIMARY KEY (`id_rol`),
  ADD UNIQUE KEY `uq_roles_nombre` (`nombre`);

--
-- Indices de la tabla `servicios`
--
ALTER TABLE `servicios`
  ADD PRIMARY KEY (`id_servicio`),
  ADD KEY `id_cliente` (`id_cliente`),
  ADD KEY `id_estado_servicio` (`id_estado_servicio`),
  ADD KEY `idx_servicios_id_usuario` (`id_usuario`),
  ADD KEY `idx_servicios_id_usuario_anulacion` (`id_usuario_anulacion`);

--
-- Indices de la tabla `usuarios`
--
ALTER TABLE `usuarios`
  ADD PRIMARY KEY (`id_usuario`),
  ADD UNIQUE KEY `uq_usuarios_nombre_usuario` (`nombre_usuario`),
  ADD KEY `idx_usuarios_id_rol` (`id_rol`);

--
-- Indices de la tabla `ventas`
--
ALTER TABLE `ventas`
  ADD PRIMARY KEY (`id_venta`),
  ADD KEY `id_cliente` (`id_cliente`),
  ADD KEY `id_medio_pago` (`id_medio_pago`),
  ADD KEY `idx_ventas_id_usuario` (`id_usuario`),
  ADD KEY `idx_ventas_id_usuario_anulacion` (`id_usuario_anulacion`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `categorias`
--
ALTER TABLE `categorias`
  MODIFY `id_categoria` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;

--
-- AUTO_INCREMENT de la tabla `clientes`
--
ALTER TABLE `clientes`
  MODIFY `id_cliente` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT de la tabla `compras_proveedores`
--
ALTER TABLE `compras_proveedores`
  MODIFY `id_compra` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT de la tabla `detalle_compras`
--
ALTER TABLE `detalle_compras`
  MODIFY `id_detalle_compra` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT de la tabla `detalle_ventas`
--
ALTER TABLE `detalle_ventas`
  MODIFY `id_detalle_venta` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT de la tabla `estados_servicio`
--
ALTER TABLE `estados_servicio`
  MODIFY `id_estado_servicio` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT de la tabla `facturas`
--
ALTER TABLE `facturas`
  MODIFY `id_factura` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `marcas`
--
ALTER TABLE `marcas`
  MODIFY `id_marca` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT de la tabla `medios_pago`
--
ALTER TABLE `medios_pago`
  MODIFY `id_medio_pago` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT de la tabla `productos`
--
ALTER TABLE `productos`
  MODIFY `id_producto` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT de la tabla `proveedores`
--
ALTER TABLE `proveedores`
  MODIFY `id_proveedor` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT de la tabla `roles`
--
ALTER TABLE `roles`
  MODIFY `id_rol` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT de la tabla `servicios`
--
ALTER TABLE `servicios`
  MODIFY `id_servicio` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT de la tabla `usuarios`
--
ALTER TABLE `usuarios`
  MODIFY `id_usuario` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT de la tabla `ventas`
--
ALTER TABLE `ventas`
  MODIFY `id_venta` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- Restricciones para tablas volcadas
--

--
-- Filtros para la tabla `compras_proveedores`
--
ALTER TABLE `compras_proveedores`
  ADD CONSTRAINT `compras_proveedores_ibfk_1` FOREIGN KEY (`id_proveedor`) REFERENCES `proveedores` (`id_proveedor`),
  ADD CONSTRAINT `fk_compras_usuario` FOREIGN KEY (`id_usuario`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_compras_usuario_anulacion` FOREIGN KEY (`id_usuario_anulacion`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE;

--
-- Filtros para la tabla `detalle_compras`
--
ALTER TABLE `detalle_compras`
  ADD CONSTRAINT `detalle_compras_ibfk_1` FOREIGN KEY (`id_compra`) REFERENCES `compras_proveedores` (`id_compra`),
  ADD CONSTRAINT `detalle_compras_ibfk_2` FOREIGN KEY (`id_producto`) REFERENCES `productos` (`id_producto`);

--
-- Filtros para la tabla `detalle_ventas`
--
ALTER TABLE `detalle_ventas`
  ADD CONSTRAINT `detalle_ventas_ibfk_1` FOREIGN KEY (`id_venta`) REFERENCES `ventas` (`id_venta`),
  ADD CONSTRAINT `detalle_ventas_ibfk_2` FOREIGN KEY (`id_producto`) REFERENCES `productos` (`id_producto`);

--
-- Filtros para la tabla `facturas`
--
ALTER TABLE `facturas`
  ADD CONSTRAINT `facturas_ibfk_1` FOREIGN KEY (`id_venta`) REFERENCES `ventas` (`id_venta`),
  ADD CONSTRAINT `facturas_ibfk_2` FOREIGN KEY (`id_servicio`) REFERENCES `servicios` (`id_servicio`),
  ADD CONSTRAINT `fk_facturas_usuario` FOREIGN KEY (`id_usuario`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_facturas_usuario_anulacion` FOREIGN KEY (`id_usuario_anulacion`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_facturas_usuario_pago` FOREIGN KEY (`id_usuario_pago`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE;

--
-- Filtros para la tabla `productos`
--
ALTER TABLE `productos`
  ADD CONSTRAINT `productos_ibfk_1` FOREIGN KEY (`id_categoria`) REFERENCES `categorias` (`id_categoria`),
  ADD CONSTRAINT `productos_ibfk_2` FOREIGN KEY (`id_marca`) REFERENCES `marcas` (`id_marca`),
  ADD CONSTRAINT `productos_ibfk_3` FOREIGN KEY (`id_proveedor`) REFERENCES `proveedores` (`id_proveedor`);

--
-- Filtros para la tabla `servicios`
--
ALTER TABLE `servicios`
  ADD CONSTRAINT `fk_servicios_usuario` FOREIGN KEY (`id_usuario`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_servicios_usuario_anulacion` FOREIGN KEY (`id_usuario_anulacion`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE,
  ADD CONSTRAINT `servicios_ibfk_1` FOREIGN KEY (`id_cliente`) REFERENCES `clientes` (`id_cliente`),
  ADD CONSTRAINT `servicios_ibfk_2` FOREIGN KEY (`id_estado_servicio`) REFERENCES `estados_servicio` (`id_estado_servicio`);

--
-- Filtros para la tabla `usuarios`
--
ALTER TABLE `usuarios`
  ADD CONSTRAINT `fk_usuarios_roles` FOREIGN KEY (`id_rol`) REFERENCES `roles` (`id_rol`) ON UPDATE CASCADE;

--
-- Filtros para la tabla `ventas`
--
ALTER TABLE `ventas`
  ADD CONSTRAINT `fk_ventas_usuario` FOREIGN KEY (`id_usuario`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_ventas_usuario_anulacion` FOREIGN KEY (`id_usuario_anulacion`) REFERENCES `usuarios` (`id_usuario`) ON UPDATE CASCADE,
  ADD CONSTRAINT `ventas_ibfk_1` FOREIGN KEY (`id_cliente`) REFERENCES `clientes` (`id_cliente`),
  ADD CONSTRAINT `ventas_ibfk_2` FOREIGN KEY (`id_medio_pago`) REFERENCES `medios_pago` (`id_medio_pago`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

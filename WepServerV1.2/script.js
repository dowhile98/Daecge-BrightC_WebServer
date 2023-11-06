function mostrarContenido(seccion) {
    // Oculta todo el contenido
    document.getElementById('contenido').innerHTML = '';

    // Muestra el contenido según la sección seleccionada
    if (seccion === 'estado') {
        document.getElementById('contenido').innerHTML = '<h2>Estado</h2><p>Contenido de Estado.</p>';
    } else if (seccion === 'configuracion') {
        document.getElementById('contenido').innerHTML = '<h2>Configuración</h2><p>Contenido de Configuración.</p>';
    } else if (seccion === 'control') {
        document.getElementById('contenido').innerHTML = '<h2>Control</h2><p>Contenido de Control.</p>';
    } else if (seccion === 'eventos') {
        document.getElementById('contenido').innerHTML = '<h2>Eventos</h2><p>Contenido de Eventos.</p>';
    }

    // Resalta la opción seleccionada en el menú
    const menuItems = document.querySelectorAll('#menu ul li');
    menuItems.forEach(item => item.classList.remove('selected'));
    document.querySelector(`#menu ul li a[onclick="mostrarContenido('${seccion}')"]`).parentElement.classList.add('selected');
}

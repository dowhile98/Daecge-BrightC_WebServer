function mostrarContenido(seccion) {
    // Oculta todo el contenido
    document.getElementById('contenido').innerHTML = '';

    // Muestra el contenido según la sección seleccionada
    if (seccion === 'estado') {
         // Crear div para mostrar el valor de los sensores de temperatura
         const sensoresDiv = document.createElement('div');
        sensoresDiv.classList.add('temperatura-container');

        const sensor1 = document.createElement('div');
        sensor1.innerText = 'Sensor de Temperatura 1: 25°C';
        sensor1.classList.add('temperatura-sensor');

        const sensor2 = document.createElement('div');
        sensor2.innerText = 'Sensor de Temperatura 2: 28°C';
        sensor2.classList.add('temperatura-sensor');

        sensoresDiv.appendChild(sensor1);
        sensoresDiv.appendChild(sensor2);
        document.getElementById('contenido').appendChild(sensoresDiv);

        // Crear div para mostrar el estado de los aires acondicionados
        const airesDiv = document.createElement('div');
        airesDiv.classList.add('aires-container');

        for (let i = 1; i <= 4; i++) {
            const aire = document.createElement('div');
            aire.classList.add('estado-aire');
            
            // Etiqueta del aire acondicionado
            const etiqueta = document.createElement('div');
            etiqueta.innerText = `AA${i}`;
            etiqueta.classList.add('etiqueta-aire');
            
            // Icono para representar el estado del aire acondicionado
            const iconoAire = document.createElement('div');
            iconoAire.classList.add('aire');
            
            // Agregar una clase específica para cada estado (encendido o apagado)
            if (i % 2 === 0) {
                iconoAire.classList.add('apagado');
            } else {
                iconoAire.classList.add('encendido');
            }

            aire.appendChild(etiqueta);
            aire.appendChild(iconoAire);
            airesDiv.appendChild(aire);
        }

        document.getElementById('contenido').appendChild(airesDiv);

    } else if (seccion === 'configuracion') {
        document.getElementById('contenido').innerHTML = '<h2>Configuración</h2><p>Contenido de Configuración.</p>';
    } else if (seccion === 'control') {
        document.getElementById('contenido').innerHTML = '<h2>Control</h2><p>Contenido de Control.</p>';
    } else if (seccion === 'eventos') {
        document.getElementById('contenido').innerHTML = '<h2>Eventos</h2><p>Contenido de Eventos.</p>';
    } else if(seccion == 'update'){

    }

    // Resalta la opción seleccionada en el menú
    const menuItems = document.querySelectorAll('#menu ul li');
    menuItems.forEach(item => item.classList.remove('selected'));
    document.querySelector(`#menu ul li a[onclick="mostrarContenido('${seccion}')"]`).parentElement.classList.add('selected');
}

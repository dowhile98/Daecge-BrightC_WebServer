document.addEventListener('DOMContentLoaded', function () {
    // Captura el evento 'submit' del formulario
    document.getElementById('config-form').addEventListener('submit', function (event) {
        event.preventDefault(); // Previene el comportamiento por defecto del formulario
  
        // Obtiene los datos del formulario
        const formData = {
            cantidadAires: document.getElementById('cantidad-aires').value,
            diasSecuencia: document.getElementById('dias-secuencia').value,
            tiempoEnfriar: document.getElementById('tiempo-enfriar').value,
            tempMinima: document.getElementById('temp-minima').value,
            tempMaxima: document.getElementById('temp-maxima').value,
            tempAlarma: document.getElementById('temp-alarma').value,
            tempBypass: document.getElementById('temp-bypass').value,
        };
  
        // Convierte los datos en formato JSON
        const jsonData = JSON.stringify(formData);
  
        // Envía la solicitud al servidor utilizando fetch
        fetch('/api/configuracion', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: jsonData
        })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            // Aquí puedes manejar la respuesta del servidor si es necesario
            console.log('Formulario enviado correctamente');
            return response.json(); // Si la respuesta del servidor es en formato JSON
        })
        .then(data => {
            // Manejar la respuesta del servidor en formato JSON si es necesario
            console.log(data);
        })
        .catch(error => {
            // Manejo de errores si la solicitud falla
            console.error('Error al enviar el formulario:', error);
        });
    });
  });
    
const configForm = document.getElementById("config-form");

configForm.addEventListener("submit", (event) => {
    event.preventDefault();

    // Obtener los valores ingresados en el formulario
    const cantidadAires = document.getElementById("cantidad-aires").value;
    const diasSecuencia = document.getElementById("dias-secuencia").value;
    const tiempoEnfriar = document.getElementById("tiempo-enfriar").value;
    const tempMinima = document.getElementById("temp-minima").value;
    const tempMaxima = document.getElementById("temp-maxima").value;
    const tempAlarma = document.getElementById("temp-alarma").value;
    const tempBypass = document.getElementById("temp-bypass").value;

    // Hacer lo que necesites con los valores ingresados
    // Por ejemplo, puedes mostrarlos en consola o enviarlos a un servidor

    // Luego de procesar los datos, puedes redirigir o mostrar un mensaje de éxito
    alert("Configuración guardada con éxito");
});


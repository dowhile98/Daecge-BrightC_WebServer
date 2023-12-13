
function enviarDatos(event) {
    // Evita el comportamiento predeterminado del formulario (no recargar la página)
    event.preventDefault();

    // Puedes realizar acciones adicionales aquí, como recopilar datos del formulario y enviarlos al servidor
    console.log("Formulario enviado");

    // Aquí puedes agregar el código para enviar los datos al servidor mediante una solicitud AJAX o cualquier otra lógica necesaria.
  }

  // Obtén el formulario por su ID y agrega el manejador de eventos al evento "submit"
document.getElementById("miFormulario").addEventListener("submit", enviarDatos);
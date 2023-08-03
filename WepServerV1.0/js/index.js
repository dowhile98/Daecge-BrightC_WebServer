const themeToggler = document.querySelector(".theme-toggler");

themeToggler.addEventListener('click', ()=>{
    document.body.classList.toggle('dark-theme-variables');
    themeToggler.querySelector('span:nth-child(1)').classList.toggle('active');
    themeToggler.querySelector('span:nth-child(2)').classList.toggle('active');
})


// const menuLinks = document.querySelectorAll(".sidebar a");

// menuLinks.forEach(link => {
//     link.addEventListener("click", (event) => {
//         event.preventDefault();
        
//         // Eliminamos la clase 'active' de todos los enlaces del menú
//         menuLinks.forEach(link => link.classList.remove("active"));
        
//         // Agregamos la clase 'active' solo al enlace que se ha hecho clic
//         link.classList.add("active");

//         // Cargar el contenido del menú correspondiente al enlace en el contenedor 'content'
//         const menuContentURL = link.getAttribute("href");
//         fetch(menuContentURL)
//             .then(response => response.text())
//             .then(htmlContent => {
//                 document.getElementById("content").innerHTML = htmlContent;
//             })
//             .catch(error => console.error(error));
//     });
// });
const menuLinks = document.querySelectorAll(".sidebar a");

menuLinks.forEach(link => {
    link.addEventListener("click", (event) => {
        event.preventDefault();
        
        // Eliminamos la clase 'active' de todos los enlaces del menú
        menuLinks.forEach(link => link.classList.remove("active"));
        
        // Agregamos la clase 'active' solo al enlace que se ha hecho clic
        link.classList.add("active");

        // Cargar el contenido del menú correspondiente al enlace en el contenedor 'content'
        const menuContentURL = link.getAttribute("href");
        fetch(menuContentURL)
            .then(response => response.text())
            .then(htmlContent => {
                document.getElementById("content").innerHTML = htmlContent;
            })
            .catch(error => console.error(error));
    });
});

// Cargar el menú "Estatus actual" por defecto al cargar la página
const defaultMenu = document.querySelector(".sidebar a[href='./status_actual.html']");
defaultMenu.classList.add("active");
const defaultMenuContentURL = defaultMenu.getAttribute("href");
fetch(defaultMenuContentURL)
    .then(response => response.text())
    .then(htmlContent => {
        document.getElementById("content").innerHTML = htmlContent;
    })
    .catch(error => console.error(error));



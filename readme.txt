LIPIANU FLORIN OVIDIU 323CA TEMA-3 PC

Pentru realizarea temei, am pornit de la implementarea laboratorului de
HTTP, creand si trimitand obiecte in format JSON, folosind biblioteca parson.
Am adaugat parametrul de token pentru functiile din helpers.c si am creat o functie noua
pentru functionalitatea de DELETE. Este similara cu GET doar ca adaptata pentru a trimite 
comanda de delete.
Pentru functiile de autentificare si register, am creat o functie auxiliara care creeaza un obiect de tip JSON.
Similar si pentru crearea unui JSON pentru carti.
Programul ruleaza cat timp nu este primita comanda exit.
Comanda register, interactioneaza cu serverul si rezerva un username, daca este disponibil.
Comanda login, verifica daca datele de autentificare sunt corecte si primeste un cookie pe care il stochez.
Comanda enter_library, primeste cookie-ul si returneaza un token.
Comanda get_books, gaseste inceputul listei de JSON-uri, si o afiseaza.
Comanda get_book, afiseaza datele unei carti de la un id dat de la tastatura,daca aceasta exista, eroare in caz contrar.
Comanda add_book, adauga o carte in lista de JSON-uri, creand un obiect de tip json pe care il trimite mai departe.
Comanda delete_book, este similara cu get_book, si inlatura din lista de JSON id-ul dorit, dat de la tastatura.
Comanda logout, sterge cookie-ul stocat la login.
Comanda exit, inchide programul.


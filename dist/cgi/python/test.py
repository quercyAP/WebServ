
import os
import sys

# Access the request method (GET or POST)
request_method = os.environ["REQUEST_METHOD"]
pathInfo = os.environ["PATH_INFO"]
httpHost = os.environ["HTTP_HOST"]


if (request_method == "GET" and pathInfo == "/env"):
    print("Content-type: text/html; charset=utf-8\n")
    print("<html>")
    print("<head>")
    print("<title>Test CGI</title>")
    print("</head>")
    print("<body>")
    print("<h1>Test CGI</h1>")
    print("<h2>Variable d'environnement</h2>")
    for cle, valeur in os.environ.items():
        print(f"<p>{cle}: {valeur}</p>")
    print("</body>")
    print("</html>")
    exit(0)


if (request_method == "POST" and pathInfo == "/upload"):
    byte_count = 0

    try:
        # Ouvrir le fichier en mode écriture binaire ('wb')
        with open("./dist/cgi/python/upload/cgiUploadedFile", 'wb') as fichier_sortie:
            # Lire les données binaires depuis stdin et écrire dans le fichier
            while True:
                # Lecture par blocs de 4096 octets (peut être ajusté)
                donnees = sys.stdin.buffer.read(4096)
                if not donnees:
                    break  # Fin de fichier (EOF)
                fichier_sortie.write(donnees)
                byte_count += len(donnees)
        print("HTTP/1.1 201 Created\r\nContent-type: text/html; charset=utf-8\n")
        print(
            f"Image enregistrée sous le nom cgiUploadedFile ({byte_count} octets recus).")
    except Exception as e:
        print(
            "HTTP/1.1 502 Error uploading File\r\nContent-type: text/html; charset=utf-8\n")
        print(f"Une erreur s'est produite : {e}")
    exit(0)


if (request_method == "GET" and pathInfo == "/download"):

    text = "HTTP/1.1 200 Belle Image\r\nContent-type: image/jpeg\r\n\r\n"
    bytes_data = text.encode("utf-8")
    sys.stdout.buffer.write(bytes_data)

    image_path = './dist/cgi/python/download.jpeg'

    try:
        with open(image_path, 'rb') as image_file:
            image_data = image_file.read()
        sys.stdout.buffer.write(image_data)
    except FileNotFoundError:
        print(f"Image file '{image_path}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    exit(0)



print("Content-type: text/html; charset=utf-8\n")
print("<html>")
print("<head>")
print("<title>Test CGI</title>")
print("</head>")
print("<body>")
print("<h1>Welcome to our python CGI!</h1>")
print("<a href=\"/python-cgi/env\">Variable d'environnement</a><br>")
print("<a href=\"/python-cgi/download\">Download</a>")

print("</body>")
print("</html>")
exit(0)

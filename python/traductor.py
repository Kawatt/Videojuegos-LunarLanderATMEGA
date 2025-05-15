def leer_y_dividir_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as f:
        contenido = f.read()

    # Eliminar llaves y espacios innecesarios
    contenido = contenido.replace('{', '').replace('}', '').strip()

    # Separar por comas
    partes = contenido.split(',')

    # Agrupar de dos en dos y dividir por 4 con redondeo
    resultado = []
    for i in range(0, len(partes), 2):
        x = int(partes[i].strip())
        y = int(partes[i+1].strip())
        resultado.append((round(x / 4), round(y / 4)))

    return resultado

def escribir_resultado(nombre_salida, datos):
    with open(nombre_salida, 'w') as f:
        f.write("{\n")
        for x, y in datos:
            f.write(f"    {{{x}, {y}}},\n")
        f.write("}\n")

# Uso del programa
entrada = 'datos.txt'
salida = 'resultado.txt'

resultado = leer_y_dividir_archivo(entrada)
escribir_resultado(salida, resultado)

print(f"✅ Resultado guardado en '{salida}'")
#!/bin/bash
function show_title() {
  echo "                                                                     
 _______ _______ _______ _______ _______ _______ _______ _______                 
|\     /|\     /|\     /|\     /|\     /|\     /|\     /|\     /|                
| +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ |                
| |   | | |   | | |   | | |   | | |   | | |   | | |   | | |   | |                
| | P | | | R | | | Á | | | C | | | T | | | I | | | C | | | A | |                
| +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ |                
|/_____\|/_____\|/_____\|/_____\|/_____\|/_____\|/_____\|/_____\|                
                                                                                 
                                                                                 
 _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ 
|\     /|\     /|\     /|\     /|\     /|\     /|\     /|\     /|\     /|\     /|
| +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ |
| |   | | |   | | |   | | |   | | |   | | |   | | |   | | |   | | |   | | |   | |
| | I | | | N | | | T | | | E | | | R | | | M | | | E | | | D | | | I | | | A | |
| +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ | +---+ |
|/_____\|/_____\|/_____\|/_____\|/_____\|/_____\|/_____\|/_____\|/_____\|/_____\|
                                                                                 
"
}

function show_plane() {
  echo "                             |
                             |
                             |
                           .-'-.
                          ' ___ '
                ---------'  .-.  '---------
_________________________'  '-'  '_________________________
 ''''''-|---|--/    \==][^',_m_,'^][==/    \--|---|-''''''
               \    /  ||/   H   \||  \    /
                '--'   OO   O|O   OO   '--'
               "
}

function show_spaceship() {
  echo '          
          /\
         //\\
        ||##||
       //##mm\\
      //##*mmm\\
     //###**mmm\\
    //###***nmmm\\
   //####***@nmmm\\
   ||####***@nnmm||
   ||####**@@@nnm||
   |______________|
   |      ULE     |
    \____________/
     |          |
    /|    /\    |\
   /_|    ||    |_\
     |          |
     |          |
     |          |
    /|    /\    |\
   / |    ||    | \
  /  |    ||    |  \
 /  /\    ||    /\  \
|__/  \   ||   /  \__|
  /____\      /____\
  |    |      |    |
  |    |______|    |
  |    | /--\ |    |
  |____|/----\|____|
    \||/ //##\\ \||/
    /##\//####\\/##\
  //##\\/####\//##\\
  ||/::\||/##\||/::\||
  \\\''///:**:\\\''///
  \\\///\::::/\\\///
    \\//\\\::///\\//
    \/\\\\..////\/
        \\\\////
        \\\///
          \\//
          \/
                    '
}

function show_menu() {
  show_title
  echo "Elige una de las siguientes opciones:"
  echo "1) Mostrar código de la práctica intermedia."
  echo "2) Compilar la práctica intermedia."
  echo "3) Ejecutar la práctica intermedia."
  echo "4) Salir."
}

function check_executable() {
  executable=1
  num_ule_programs=0
  # Checks if there are .ule executable programs in current directory
  for i in *; do
    if [[ $i == *.ule ]]; then
      num_ule_programs=`expr $num_ule_programs + 1`
      if test -f "$i" && test -x "$i"; then
        echo "$i es ejecutable."
      else
        executable=0
      fi
    fi
  done

  # If there are no .ule programs, there is no way to execute the program 
  if test $num_ule_programs -eq 0; then
    executable=0
  fi
}

function get_passengers() {
  re="^[0-9]+$"

  while true; do
    echo "Introduce el número de asistentes de vuelo: "
    read num_flight_assistants

    if ! [[ $num_flight_assistants =~ $re ]]; then
      echo "Error, no se ha introducido un número válido."
    elif test $num_flight_assistants -lt 1; then
      echo "Error, el número de asistentes no es válido."
    else
      if test $num_flight_assistants -ge 50; then
        echo "¿No crees que son demasiados asistentes para un avión?"
        sleep 2
        echo "Preparando el transbordador espacial..."
        sleep 2
        show_spaceship
      else
        echo "Preparando el avión..."
        show_plane
      fi
      break
    fi
  done
}

while true; do
  show_menu
  read option
  case $option in
  1)
    # Show project code
    cat *.c
    ;;
  2)
    # Compile project code
    gcc *.c -o so-practica-intermedia.ule
    echo "Fin de la compilación." 
    ;;
  3)
    check_executable

    if test $executable -eq 1; then
      get_passengers

      ./*.ule $num_flight_assistants
      sleep 1
    else
      echo "Error, el programa no es ejecutable o no existe."
    fi

    ;;
  4)
    exit 0
    ;;
  *)
    echo "Error, opción no disponible."
    ;;
  esac

  echo ""
done

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

void showTitle()
{
  printf(" _______  ___      ___   _______  __   __  _______\n");
  printf("|       ||   |    |   | |       ||  | |  ||       |\n");
  printf("|    ___||   |    |   | |    ___||  |_|  ||_     _|\n");
  printf("|   |___ |   |    |   | |   | __ |       |  |   |\n");
  printf("|    ___||   |___ |   | |   ||  ||       |  |   |\n");
  printf("|   |    |       ||   | |   |_| ||   _   |  |   |\n");
  printf("|___|    |_______||___| |_______||__| |__|  |___|\n\n");
  printf(" _______  ___   __   __  __   __  ___      _______  _______  _______  ______\n");
  printf("|       ||   | |  |_|  ||  | |  ||   |    |   _   ||       ||       ||    _ |\n");
  printf("|  _____||   | |       ||  | |  ||   |    |  |_|  ||_     _||   _   ||   | ||\n");
  printf("| |_____ |   | |       ||  |_|  ||   |    |       |  |   |  |  | |  ||   |_||_\n");
  printf("|_____  ||   | |       ||       ||   |___ |       |  |   |  |  |_|  ||    __  |\n");
  printf(" _____| ||   | | ||_|| ||       ||       ||   _   |  |   |  |       ||   |  | |\n");
  printf("|_______||___| |_|   |_||_______||_______||__| |__|  |___|  |_______||___|  |_|\n\n");
}

int getRandomNumberInRange(int min, int max)
{
  // Get seed for random numbers whenever the function is called
  srand(getpid());
  return rand() % (max - min + 1) + min;
}

void handlerTechnician(int sig)
{
  printf("El técnico ha recibido la orden del coordinador de comprobar la viabilidad del vuelo.\n");

  // Wait between 3 to 6 seconds
  sleep(getRandomNumberInRange(3, 6));

  // Check if flight is viable (60% chance)
  if (getRandomNumberInRange(1, 6) == 1)
  {
    // The flight is not viable
    exit(0);
  }
  else
  {
    // The flight is viable
    exit(1);
  }
}

void handlerManager(int sig)
{
  printf("El encargado ha recibido la orden del coordinador de comprobar si hay overbooking.\n");

  // Wait 2 seconds
  sleep(2);

  // Check if there is overbooking (50% chance)
  if (getRandomNumberInRange(0, 1) == 1)
  {
    // There is overbooking
    exit(1);
  }
  else
  {
    // There is no overbooking
    exit(0);
  }
}

void handlerAssistants(int sig)
{
  // Wait between 3 to 6 seconds
  sleep(getRandomNumberInRange(3, 6));

  // Exits with a random amount of boarded passengers
  exit(getRandomNumberInRange(20, 30));
}

int main(int argc, char const *argv[])
{
  // Checking whether the amount of flight assistants is correct relies purely on the bash script

  showTitle();

  // Set up sigaction
  struct sigaction ss;

  int numFlightAssistants = atoi(argv[1]);
  pid_t currentP, planeTechnician, flightManager, *flightAssistants;
  flightAssistants = (pid_t *)malloc(sizeof(pid_t) * numFlightAssistants);

  // Coordinator creates plane technician and flight manager processes
  currentP = fork();
  if (currentP == -1)
  {
    perror("Error creando nuevo proceso, abortando ejecución...");
    return 1;
  }
  planeTechnician = currentP;
  if (currentP != 0)
  {
    currentP = fork();
    if (currentP == -1)
    {
      perror("Error creando nuevo proceso, abortando ejecución...");
      return 1;
    }
    flightManager = currentP;
  }

  // Technician's work
  if (planeTechnician == 0)
  {
    ss.sa_handler = &handlerTechnician;
    if (sigaction(SIGUSR1, &ss, NULL) == -1)
    {
      perror("Error relativo a envío de señales, abortando ejecución...");
      return 1;
    }
    // The technician waits for coordinator's signal to check the plane
    pause();
  }
  // Flight manager's work
  else if (flightManager == 0)
  {
    ss.sa_handler = &handlerManager;
    if (sigaction(SIGUSR1, &ss, NULL) == -1)
    {
      perror("Error relativo a envío de señales, abortando ejecución...");
      return 1;
    }
    // The manager waits for coordinator's signal to check for overbooking
    pause();
  }
  // Coordinator's work
  else
  {
    // Coordinator waits for their workers to be ready for signal reception
    sleep(1);

    kill(planeTechnician, SIGUSR1);
    int flightViable;

    int wstatus;
    wait(&wstatus);
    if (WIFEXITED(wstatus))
    {
      flightViable = WEXITSTATUS(wstatus);
    }

    if (flightViable == 1)
    {
      printf("El técnico ha determinado que el vuelo es viable.\n");

      int numPassengers = 0;

      // The flight is viable, the manager should now check if there is overbooking
      kill(flightManager, SIGUSR1);
      int overbooking;

      wait(&wstatus);
      if (WIFEXITED(wstatus))
      {
        overbooking = WEXITSTATUS(wstatus);
      }

      if (overbooking == 1)
      {
        // There is overbooking
        printf("El vuelo está sufriendo de overbooking.\n");
        numPassengers -= 10;
      }
      else
      {
        printf("El vuelo no tiene overbooking.\n");
      }

      // Create flight assistant processes
      for (int i = 0; i < numFlightAssistants; i++)
      {
        if (currentP != 0)
        {
          currentP = fork();
          if (currentP == -1)
          {
            perror("Error creando nuevo proceso, abortando ejecución...");
            return 1;
          }
          *(flightAssistants + i) = currentP;
        }
      }

      // Flight assistants' work
      if (currentP == 0)
      {
        ss.sa_handler = &handlerAssistants;
        if (sigaction(SIGUSR2, &ss, NULL) == -1)
        {
          perror("Error relativo a envío de señales, abortando ejecución...");
          return 1;
        }
        // The flight assistants wait for coordinator's signal to initiate the boarding of the plane
        pause();
      }
      // Coordinator's work
      else
      {
        // The coordinator waits 2 seconds before signaling the flight assistants
        sleep(2);

        for (int i = 0; i < numFlightAssistants; i++)
        {
          kill(*(flightAssistants + i), SIGUSR2);
        }

        // Each assistant will start boarding their passengers

        while (1)
        {
          currentP = wait(&wstatus);

          // Check if there are no assistants left to keep boarding
          if (currentP == -1)
          {
            break;
          }

          if (WIFEXITED(wstatus))
          {
            // Get assistant's index
            int assistantIndex;
            for (int i = 0; i < numFlightAssistants; i++)
            {
              if (*(flightAssistants + i) == currentP)
              {
                assistantIndex = i;
              }
            }

            // Get boarded passengers of the assistant
            int boardedPassengers = WEXITSTATUS(wstatus);
            numPassengers += boardedPassengers;

            printf("El asistente %d ha embarcado a %d pasajeros.\n", assistantIndex + 1, boardedPassengers);
          }
        }

        if (overbooking == 1)
        {
          printf("Debido al overbooking, el último asistente no ha logrado embarcar a 10 pasajeros.\n");
        }

        printf("El total de pasajeros embarcados es: %d\n", numPassengers);
      }
    }
    else
    {
      // The flight is not viable, we kill all the processes we created before exiting
      kill(SIGINT, planeTechnician);
      kill(SIGINT, flightManager);
      printf("El vuelo ha sido cancelado debido a que el técnico ha rechazado su viabilidad.");
    }
  }

  free(flightAssistants);
  return 0;
}

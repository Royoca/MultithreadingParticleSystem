#include <SFML/Graphics.hpp>
#include "ParticleSystem.h"
#include "Constants.h"
#include <windows.h>
#include <future>

int main()
{

  sf::RenderWindow window(sf::VideoMode((int)k_screen_x, (int)k_screen_y), "Particles Jesus Royo");

  const unsigned int n_threads = std::thread::hardware_concurrency();

  unsigned int particlesPerThread = kn_particles / n_threads;
  std::vector<std::future<void>> futures(n_threads);

  ParticleSystem particles(kn_particles);

  sf::Clock deltaClock; 
  sf::Time dt;

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    for (unsigned int i = 0; i < n_threads; ++i) {
      unsigned int start = i * particlesPerThread;
      unsigned int end = (i + 1) * particlesPerThread;
      futures[i] = std::async(std::launch::async, ParticleSystem::UpdateParticles, std::ref(particles), dt.asSeconds(), start, end);
    }

    for (auto& f : futures) {
      f.get();
    }

    window.clear(sf::Color(30, 30, 30));
    particles.draw(window);
    window.display(); 
    dt = deltaClock.restart();
  }

  return 0;
}
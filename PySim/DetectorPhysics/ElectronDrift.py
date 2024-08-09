import numpy as np
from typing import Tuple
from tqdm import tqdm

# Physical constants
BOLTZMANN = 1.380649e-23  # Boltzmann constant in J/K
ELEMENTARY_CHARGE = 1.602176634e-19  # Elementary charge in C

def calculate_diffusion_constant(mobility: float, temperature: float) -> float:
    """
    Calculate the diffusion constant using the Einstein relation.
    
    Args:
    - mobility: Charge carrier mobility in m^2/(V*s)
    - temperature: Temperature in Kelvin
    
    Returns:
    - Diffusion constant in m^2/s
    """
    return mobility * BOLTZMANN * temperature / ELEMENTARY_CHARGE

def drift_velocity(electric_field: np.ndarray, mobility: float) -> np.ndarray:
    """Calculate drift velocity based on electric field and mobility."""
    return mobility * electric_field

def is_in_detector(positions: np.ndarray, detector_bounds: Tuple[np.ndarray, np.ndarray]) -> np.ndarray:
    """Check if positions are within the detector bounds."""
    min_bound, max_bound = detector_bounds
    return np.all((positions >= min_bound) & (positions <= max_bound), axis=1)

def random_direction(num_particles):
    """Generate random unit vectors in 3D space."""
    theta = np.random.uniform(0, np.pi, num_particles)
    phi = np.random.uniform(0, 2*np.pi, num_particles)
    x = np.sin(theta) * np.cos(phi)
    y = np.sin(theta) * np.sin(phi)
    z = np.cos(theta)
    return np.column_stack((x, y, z))

def drift_charges(
    electric_field_func,
    mobility: float,
    start_position: np.ndarray,
    num_electrons: int,
    diffusion_constant: float,
    detector_bounds: Tuple[np.ndarray, np.ndarray],
    dt: float = 1e-9,
    max_steps: int = 1000
) -> np.ndarray:
    """
    Simulate charge drift and diffusion with random direction steps.
    
    Returns:
    - Array of final x-positions for all electrons
    """
    positions = np.tile(start_position, (num_electrons, 1))
    min_bound, max_bound = detector_bounds
    
    for _ in tqdm(range(max_steps)):
        active_electrons = is_in_detector(positions, detector_bounds)
        if not np.any(active_electrons):
            break
        
        active_positions = positions[active_electrons]
        num_active = len(active_positions)
        
        e_field = np.array([electric_field_func(pos) for pos in active_positions])
        drift_step = drift_velocity(e_field, mobility) * dt
        
        # Generate random directions and magnitudes for diffusion
        diff_directions = random_direction(num_active)
        diff_magnitudes = np.random.normal(0, np.sqrt(2 * diffusion_constant * dt), num_active)
        diff_step = diff_directions * diff_magnitudes[:, np.newaxis]
        
        new_positions = active_positions + drift_step + diff_step
        
        # Reflect particles at boundaries
        for i in range(3):  # For each dimension (x, y, z)
            over_max = new_positions[:, i] > max_bound[i]
            under_min = new_positions[:, i] < min_bound[i]
            new_positions[over_max, i] = 2 * max_bound[i] - new_positions[over_max, i]
            new_positions[under_min, i] = 2 * min_bound[i] - new_positions[under_min, i]
        
        # Ensure particles stay within detector bounds (in case of double reflection)
        new_positions = np.clip(new_positions, min_bound, max_bound)
        
        positions[active_electrons] = new_positions
    
    return positions

def example_electric_field(position):
    """Example electric field function. Replace with your actual field."""
    return np.array([0, 0, 1e5])  # Constant field in z-direction
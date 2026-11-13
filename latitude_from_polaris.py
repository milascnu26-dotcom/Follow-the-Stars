import numpy as np

# Polaris er altitude (horizon theke angle) prai tomar latitude er soman
def estimate_latitude(altitude_deg):
    if not 0 <= altitude_deg <= 90:
        raise ValueError("Altitude must be between 0 and 90 degrees")
    return altitude_deg

try:
    angle = float(input("Polaris er altitude koto degree? "))
    lat = estimate_latitude(angle)
    print(f"Tomar approximate latitude: {lat:.1f} degree North")
    # Prai koto km uttore equator theke (1 degree ~ 111 km)
    print(f"Equator theke dooratto: {np.round(lat * 111)} km")
except ValueError as e:
    print("Invalid input:", e)

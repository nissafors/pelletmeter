__copyright__ = "Copyright (C) 2021 Andreas Andersson"

"""Settings for the Pelletmeter web service."""

"""Raw measurement min and max allowed values in cm.
Leave room for error! Any measurement outside the range will be ignored!"""
measurement_bounds = (20, 150)

"""Converted levels corresponding to raw measurement min and max."""
level_bounds = (130, 0)

"""Levels above this value are considered high."""
level_high = 90

"""Levels below this value are considered low."""
level_low = 40

"""Print debug info."""
verbose = True
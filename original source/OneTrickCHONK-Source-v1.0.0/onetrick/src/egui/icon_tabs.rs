/*
   ┏━━━┓╱╱╱╱╱┏┓╱╱┏┓╱╱╱╱╱┏┓╱╱
   ┃┏━┓┃╱╱╱╱╱┃┃╱╱┃┃╱╱╱╱╱┃┃╱╱╱
   ┃┗━┛┣┓┏┳━━┫┃┏┓┃┃╱╱┏━━┫┗━┳━━┓
   ┃┏━━┫┃┃┃┏┓┃┗┛┃┃┃╱┏┫┏┓┃┏┓┃━━┫
   ┃┃╱╱┃┗┛┃┃┃┃┏┓┃┃┗━┛┃┏┓┃┗┛┣━━┃
   ┗┛╱╱┗━━┻┛┗┻┛┗┛┗━━━┻┛┗┻━━┻━━┛
    ━━━━━━━━━━━━━━━━━━━━━━━━━━

    Copyright (c) 2023, 2024 Punk Labs LLC

    This section is part of OneTrick

    OneTrick is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    OneTrick is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    OneTrick.  If not, see <http://www.gnu.org/licenses/>.
*/

use std::vec;

use nih_plug_egui::egui::{
    vec2,
    Align,
    Align2,
    Color32,
    Rect,
    Response,
    Rounding,
    Sense,
    Ui,
    Vec2,
    Widget,
    Layout,
    /*
    Shape,
    epaint::{
        Shadow,
    },
    */
};

use super::icons::{Dingbat, IconDrawer};
use crate::color::InterpolateColors;


/// Orientation of an IconTabs
#[derive(Clone)]
pub enum IconTabsOrientation {
    Horizontal,
    Vertical,
}

/// Style info for an IconTabs
#[derive(Clone)]
pub struct IconTabsStyle {
    /// Orientation of the tabs
    pub orientation: IconTabsOrientation,

    /// Width of the background panel
    pub panel_length: Option<f32>,

    /// Height of the background panel
    pub panel_width: Option<f32>,

    /// Color of the backgrond panel
    pub panel_color: Color32,

    /// Rounding of the background panel
    pub panel_rounding: Rounding,

    /// Color of a tab icon
    pub color: Color32,
    
    /// Color of a tab icon when active
    pub color_active: Color32,

    /// Color of a tab background when active
    pub tab_color_active: Color32,

    /// Rounding of a tab background
    pub tab_rounding: Rounding,

    /// Amount to expand the tab when hovering
    pub tab_hover_resize: Vec2,

    /// XY Padding of the Icon
    pub icon_padding: f32,
}
impl Default for IconTabsStyle {
    fn default() -> Self {
        Self {
            orientation: IconTabsOrientation::Horizontal,
            panel_length: None,
            panel_width: Some(64.0),
            panel_color: Color32::LIGHT_GRAY,
            panel_rounding: Rounding::ZERO,
            color: Color32::DARK_GRAY,
            color_active: Color32::DARK_RED,
            tab_color_active: Color32::WHITE,
            tab_rounding: Rounding::same(5.0),
            icon_padding: 10.0,
            tab_hover_resize: Vec2::new(0.0, 2.0),
        }
    }
}

/// Style info for a single Tab
#[derive(Clone)]
pub struct IconTabStyle {

    /// Dingbat Icon to draw on the tab
    pub dingbat: Dingbat,

    /// Icon color
    pub color: Option<Color32>,

    /// Icon background color
    pub background_color: Option<Color32>,

    /// Amount to zoom when hovering
    pub zoom: f32,
}
impl Default for IconTabStyle {
    fn default() -> Self {
        Self {
            dingbat: Dingbat::Info,
            color: None,
            background_color: None,
            zoom: 1.0,
        }
    }
}
impl IconTabStyle {

    /// Set the Icon to a Dingbat
    pub fn with_dingbat(mut self, dingbat: Dingbat) -> Self {
        self.dingbat = dingbat;
        self
    }

    /// set the Icon color
    pub fn with_color(mut self, color: Color32) -> Self {
        self.color = Some(color);
        self
    }

    /// set an optional background color
    pub fn with_background_color(mut self, background_color: Color32) -> Self {
        self.background_color = Some(background_color);
        self
    }
    
    /// set the amount to zoom when hovering
    pub fn with_zoom(mut self, zoom: f32) -> Self {
        self.zoom = zoom;
        self
    }
}

/// An Icon Tabs UI Element
pub struct IconTabs<'a> {
    active_index: &'a mut u32,
    style: IconTabsStyle,
    tabs: Vec<IconTabStyle>,
}

#[allow(unused)]
impl<'a> IconTabs<'a> {
    /// Returns a new IconTabs
    pub fn new(active_index: &'a mut u32, style: IconTabsStyle, tabs: Vec<IconTabStyle>) -> Self {
        Self {
            active_index,
            style,
            tabs,
        }
    }
}

impl<'a> Widget for IconTabs<'a> {
    fn ui(self, ui: &mut Ui) -> Response {
        let available_size = ui.available_size();
        let available_length = match self.style.orientation {
            IconTabsOrientation::Horizontal => available_size.x,
            IconTabsOrientation::Vertical   => available_size.y,
        };
        let panel_length = self.style.panel_length.unwrap_or(available_length);
        let panel_width = self.style.panel_width.unwrap_or(64.0);
        let panel_size = match self.style.orientation {
            IconTabsOrientation::Horizontal => Vec2::new(panel_length, panel_width),
            IconTabsOrientation::Vertical => Vec2::new(panel_width, panel_length)
        };
        let tab_count = self.tabs.len();
        let tab_size = match self.style.orientation {
            IconTabsOrientation::Horizontal => Vec2::new(panel_size.x / (tab_count as f32), panel_size.y),
            IconTabsOrientation::Vertical  => Vec2::new(panel_size.x, panel_size.y / (tab_count as f32))
        };
        //let tab_size = Vec2::new(tab_width, tab_height);
        let topleft = ui.next_widget_position();
        let rect = Rect::from_min_size(topleft, panel_size);
        // ui.allocate_ui_at_rect(rect, |ui| {
        let layout = match self.style.orientation {
            IconTabsOrientation::Horizontal => Layout::left_to_right(Align::Min),
            IconTabsOrientation::Vertical => Layout::top_down(Align::Min),
        };
        ui.allocate_ui_with_layout(rect.size(), layout, |ui| {
            let color = self.style.panel_color;
            if color.a() > 0 {
                ui.painter()
                    .rect_filled(rect, self.style.panel_rounding, color);
            }
            // ui.horizontal(|ui| {
                ui.spacing_mut().item_spacing = vec2(0.0, 0.0);
                let mut rects = vec![];
                for _tab in self.tabs.iter() {
                    let (rect, response) =
                        ui.allocate_exact_size(tab_size, Sense::click());
                    rects.push((rect, response));
                }

                // Draw Optional Background
                for (index, tab) in self.tabs.iter().enumerate() {
                    let (rect, _response) = rects.get_mut(index).unwrap();
                    if let Some(background_color) = tab.background_color {
                        let corners = self.style.panel_rounding;
                        let rounding = if index==0 {
                            match self.style.orientation {
                                IconTabsOrientation::Horizontal => Rounding{nw:corners.nw, ne:0.0, sw:corners.sw, se:0.0},
                                IconTabsOrientation::Vertical => Rounding{nw:corners.nw, ne:corners.ne, sw:0.0, se:0.0},
                            }
                        } else if index==self.tabs.len()-1 {
                            match self.style.orientation {
                                IconTabsOrientation::Horizontal => Rounding{nw:0.0, ne:corners.ne, sw:0.0, se:corners.se},
                                IconTabsOrientation::Vertical => Rounding{nw:0.0, ne:0.0, sw:corners.sw, se:corners.se},
                            }
                        } else {
                            Rounding::ZERO
                        };
                        ui.painter().rect_filled(*rect, rounding, background_color);
                    }
                }
                for (index, tab) in self.tabs.iter().enumerate() {
                    let (rect, response) = rects.get_mut(index).unwrap();
                    let rect = *rect;
                    
                    let index = index as u32;
                    let is_active = index == *self.active_index;

                    if response.clicked() {
                        response.request_focus();
                    }
                    
                    let is_active_or_hovered = is_active || response.hovered();
                    let hover_amount = ui.ctx().animate_bool_with_time(
                        response.id,//.with("hover"),
                        response.hovered(),
                        0.100,
                    );

                    // Draw Tab
                    let color = if is_active_or_hovered {
                        self.style.tab_color_active
                    } else if hover_amount > 0.0 {
                        Color32::TRANSPARENT.interpolate_to(self.style.tab_color_active, hover_amount)
                    } else {
                        Color32::TRANSPARENT
                    };

                    let expanded_rect = rect.expand2(self.style.tab_hover_resize * hover_amount);

                    /*
                    if is_active {
                        let shadow = Shadow{extrusion: 6.0, color: Color32::from_black_alpha(32)};
                        let shadow = shadow.tessellate(expanded_rect, self.style.panel_rounding);
                        let shadow = Shape::Mesh(shadow);
                        ui.painter().add(shadow);
                    }
                    */

                    if color.a() > 0 {
                        ui.painter().rect_filled(
                            expanded_rect,
                            self.style.tab_rounding,
                            color,
                        );
                    }

                    // Draw Icon
                    let color = if is_active_or_hovered {
                        self.style.color_active
                    } else {
                        tab.color.unwrap_or(self.style.color)
                    };

                    let tab_adjusted_size = match self.style.orientation {
                        IconTabsOrientation::Horizontal => tab_size.y,
                        IconTabsOrientation::Vertical => tab_size.x
                    };
                    ui.painter().dingbat(
                        rect.center(),
                        Align2::CENTER_CENTER,
                        tab.dingbat,
                        (tab_adjusted_size - self.style.icon_padding * 2.0) * tab.zoom,
                        color,
                    );

                    if response.has_focus() {
                        let focus_stroke = ui.style().visuals.selection.stroke;
                        if !focus_stroke.is_empty() {
                            ui.painter().rect_stroke(expanded_rect, self.style.tab_rounding, focus_stroke);
                        }
                    }

                    if response.clicked() {
                        *self.active_index = index;
                    }
                }
            //});
        })
        .response
    }
}

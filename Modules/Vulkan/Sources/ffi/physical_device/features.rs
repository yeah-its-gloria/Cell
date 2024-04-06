// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use alloc::{vec, vec::IntoIter};
use core::{ffi::c_void, ptr::null_mut};

use crate::ffi::{Bool32, StructureType, VK_FALSE};

#[repr(C)]
pub struct PhysicalDeviceDynamicRenderingFeatures {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub dynamic_rendering: Bool32,
}

#[repr(C)]
pub struct PhysicalDeviceExtendedDynamicStateFeatures {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub extended_dynamic_state: Bool32,
}

#[repr(C)]
pub struct PhysicalDeviceFeatures {
    pub robust_buffer_access: Bool32,
    pub full_draw_index_uint32: Bool32,
    pub image_cube_array: Bool32,
    pub independent_blend: Bool32,
    pub geometry_shader: Bool32,
    pub tessellation_shader: Bool32,
    pub sample_rate_shading: Bool32,
    pub dual_src_blend: Bool32,
    pub logic_op: Bool32,
    pub multi_draw_indirect: Bool32,
    pub draw_indirect_first_instance: Bool32,
    pub depth_clamp: Bool32,
    pub depth_bias_clamp: Bool32,
    pub fill_mode_non_solid: Bool32,
    pub depth_bounds: Bool32,
    pub wide_lines: Bool32,
    pub large_points: Bool32,
    pub alpha_to_one: Bool32,
    pub multi_viewport: Bool32,
    pub sampler_anisotropy: Bool32,
    pub texture_compression_etc2: Bool32,
    pub texture_compression_astc_ldr: Bool32,
    pub texture_compression_bc: Bool32,
    pub occlusion_query_precise: Bool32,
    pub pipeline_statistics_query: Bool32,
    pub vertex_pipeline_stores_and_atomics: Bool32,
    pub fragment_stores_and_atomics: Bool32,
    pub shader_tessellation_and_geometry_point_size: Bool32,
    pub shader_image_gather_extended: Bool32,
    pub shader_storage_image_extended_formats: Bool32,
    pub shader_storage_image_multisample: Bool32,
    pub shader_storage_image_read_without_format: Bool32,
    pub shader_storage_image_write_without_format: Bool32,
    pub shader_uniform_buffer_array_dynamic_indexing: Bool32,
    pub shader_sampled_image_array_dynamic_indexing: Bool32,
    pub shader_storage_buffer_array_dynamic_indexing: Bool32,
    pub shader_storage_image_array_dynamic_indexing: Bool32,
    pub shader_clip_distance: Bool32,
    pub shader_cull_distance: Bool32,
    pub shader_float64: Bool32,
    pub shader_int64: Bool32,
    pub shader_int16: Bool32,
    pub shader_resource_residency: Bool32,
    pub shader_resource_min_lod: Bool32,
    pub sparse_binding: Bool32,
    pub sparse_residency_buffer: Bool32,
    pub sparse_residency_image_2d: Bool32,
    pub sparse_residency_image_3d: Bool32,
    pub sparse_residency_2_samples: Bool32,
    pub sparse_residency_4_samples: Bool32,
    pub sparse_residency_8_samples: Bool32,
    pub sparse_residency_16_samples: Bool32,
    pub sparse_residency_aliased: Bool32,
    pub variable_multisample_rate: Bool32,
    pub inherited_queries: Bool32,
}

/// Implements VkPhysicalDeviceFeatures2
#[repr(C)]
pub struct PhysicalDeviceFeaturesEx {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub features: PhysicalDeviceFeatures,
}

impl Default for PhysicalDeviceDynamicRenderingFeatures {
    fn default() -> Self {
        PhysicalDeviceDynamicRenderingFeatures {
            s_type: StructureType::PhysicalDeviceDynamicRenderingFeatures,
            next: null_mut(),
            dynamic_rendering: VK_FALSE,
        }
    }
}

impl Default for PhysicalDeviceExtendedDynamicStateFeatures {
    fn default() -> Self {
        PhysicalDeviceExtendedDynamicStateFeatures {
            s_type: StructureType::PhysicalDeviceExtendedDynamicStateFeatures,
            next: null_mut(),
            extended_dynamic_state: VK_FALSE,
        }
    }
}

impl Default for PhysicalDeviceFeatures {
    fn default() -> Self {
        PhysicalDeviceFeatures {
            robust_buffer_access: VK_FALSE,
            full_draw_index_uint32: VK_FALSE,
            image_cube_array: VK_FALSE,
            independent_blend: VK_FALSE,
            geometry_shader: VK_FALSE,
            tessellation_shader: VK_FALSE,
            sample_rate_shading: VK_FALSE,
            dual_src_blend: VK_FALSE,
            logic_op: VK_FALSE,
            multi_draw_indirect: VK_FALSE,
            draw_indirect_first_instance: VK_FALSE,
            depth_clamp: VK_FALSE,
            depth_bias_clamp: VK_FALSE,
            fill_mode_non_solid: VK_FALSE,
            depth_bounds: VK_FALSE,
            wide_lines: VK_FALSE,
            large_points: VK_FALSE,
            alpha_to_one: VK_FALSE,
            multi_viewport: VK_FALSE,
            sampler_anisotropy: VK_FALSE,
            texture_compression_etc2: VK_FALSE,
            texture_compression_astc_ldr: VK_FALSE,
            texture_compression_bc: VK_FALSE,
            occlusion_query_precise: VK_FALSE,
            pipeline_statistics_query: VK_FALSE,
            vertex_pipeline_stores_and_atomics: VK_FALSE,
            fragment_stores_and_atomics: VK_FALSE,
            shader_tessellation_and_geometry_point_size: VK_FALSE,
            shader_image_gather_extended: VK_FALSE,
            shader_storage_image_extended_formats: VK_FALSE,
            shader_storage_image_multisample: VK_FALSE,
            shader_storage_image_read_without_format: VK_FALSE,
            shader_storage_image_write_without_format: VK_FALSE,
            shader_uniform_buffer_array_dynamic_indexing: VK_FALSE,
            shader_sampled_image_array_dynamic_indexing: VK_FALSE,
            shader_storage_buffer_array_dynamic_indexing: VK_FALSE,
            shader_storage_image_array_dynamic_indexing: VK_FALSE,
            shader_clip_distance: VK_FALSE,
            shader_cull_distance: VK_FALSE,
            shader_float64: VK_FALSE,
            shader_int64: VK_FALSE,
            shader_int16: VK_FALSE,
            shader_resource_residency: VK_FALSE,
            shader_resource_min_lod: VK_FALSE,
            sparse_binding: VK_FALSE,
            sparse_residency_buffer: VK_FALSE,
            sparse_residency_image_2d: VK_FALSE,
            sparse_residency_image_3d: VK_FALSE,
            sparse_residency_2_samples: VK_FALSE,
            sparse_residency_4_samples: VK_FALSE,
            sparse_residency_8_samples: VK_FALSE,
            sparse_residency_16_samples: VK_FALSE,
            sparse_residency_aliased: VK_FALSE,
            variable_multisample_rate: VK_FALSE,
            inherited_queries: VK_FALSE,
        }
    }
}

impl Default for PhysicalDeviceFeaturesEx {
    fn default() -> Self {
        PhysicalDeviceFeaturesEx {
            s_type: StructureType::PhysicalDeviceFeatures,
            next: null_mut(),
            features: PhysicalDeviceFeatures::default(),
        }
    }
}

impl IntoIterator for PhysicalDeviceFeatures {
    type Item = Bool32;
    type IntoIter = IntoIter<Self::Item>;

    fn into_iter(self) -> Self::IntoIter {
        vec![
            self.robust_buffer_access,
            self.full_draw_index_uint32,
            self.image_cube_array,
            self.independent_blend,
            self.geometry_shader,
            self.tessellation_shader,
            self.sample_rate_shading,
            self.dual_src_blend,
            self.logic_op,
            self.multi_draw_indirect,
            self.draw_indirect_first_instance,
            self.depth_clamp,
            self.depth_bias_clamp,
            self.fill_mode_non_solid,
            self.depth_bounds,
            self.wide_lines,
            self.large_points,
            self.alpha_to_one,
            self.multi_viewport,
            self.sampler_anisotropy,
            self.texture_compression_etc2,
            self.texture_compression_astc_ldr,
            self.texture_compression_bc,
            self.occlusion_query_precise,
            self.pipeline_statistics_query,
            self.vertex_pipeline_stores_and_atomics,
            self.fragment_stores_and_atomics,
            self.shader_tessellation_and_geometry_point_size,
            self.shader_image_gather_extended,
            self.shader_storage_image_extended_formats,
            self.shader_storage_image_multisample,
            self.shader_storage_image_read_without_format,
            self.shader_storage_image_write_without_format,
            self.shader_uniform_buffer_array_dynamic_indexing,
            self.shader_sampled_image_array_dynamic_indexing,
            self.shader_storage_buffer_array_dynamic_indexing,
            self.shader_storage_image_array_dynamic_indexing,
            self.shader_clip_distance,
            self.shader_cull_distance,
            self.shader_float64,
            self.shader_int64,
            self.shader_int16,
            self.shader_resource_residency,
            self.shader_resource_min_lod,
            self.sparse_binding,
            self.sparse_residency_buffer,
            self.sparse_residency_image_2d,
            self.sparse_residency_image_3d,
            self.sparse_residency_2_samples,
            self.sparse_residency_4_samples,
            self.sparse_residency_8_samples,
            self.sparse_residency_16_samples,
            self.sparse_residency_aliased,
            self.variable_multisample_rate,
            self.inherited_queries,
        ]
        .into_iter()
    }
}

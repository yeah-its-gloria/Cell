use std::ffi::c_void;
use std::ptr::null_mut;

use super::constants::{MAX_PHYSICAL_DEVICE_NAME_SIZE, UUID_SIZE};
use super::enumerations::{PhysicalDeviceType, SampleCountFlag, StructureType};
use super::structures::{
    PhysicalDeviceDynamicRenderingFeatures, PhysicalDeviceFeatures, PhysicalDeviceLimits,
    PhysicalDeviceProperties, PhysicalDeviceSparseProperties,
};
use super::types::VK_FALSE;

impl Default for PhysicalDeviceLimits {
    fn default() -> Self {
        PhysicalDeviceLimits {
            max_image_dimension_1d: 0,
            max_image_dimension_2d: 0,
            max_image_dimension_3d: 0,
            max_image_dimension_cube: 0,
            max_image_array_layers: 0,
            max_texel_buffer_elements: 0,
            max_uniform_buffer_range: 0,
            max_storage_buffer_range: 0,
            max_push_constants_size: 0,
            max_memory_allocation_count: 0,
            max_sampler_allocation_count: 0,
            buffer_image_granularity: 0,
            sparse_address_space_size: 0,
            max_bound_descriptor_sets: 0,
            max_per_stage_descriptor_samplers: 0,
            max_per_stage_descriptor_uniform_buffers: 0,
            max_per_stage_descriptor_storage_buffers: 0,
            max_per_stage_descriptor_sampled_images: 0,
            max_per_stage_descriptor_storage_images: 0,
            max_per_stage_descriptor_input_attachments: 0,
            max_per_stage_resources: 0,
            max_descriptor_set_samplers: 0,
            max_descriptor_set_uniform_buffers: 0,
            max_descriptor_set_uniform_buffers_dynamic: 0,
            max_descriptor_set_storage_buffers: 0,
            max_descriptor_set_storage_buffers_dynamic: 0,
            max_descriptor_set_sampled_images: 0,
            max_descriptor_set_storage_images: 0,
            max_descriptor_set_input_attachments: 0,
            max_vertex_input_attributes: 0,
            max_vertex_input_bindings: 0,
            max_vertex_input_attribute_offset: 0,
            max_vertex_input_binding_stride: 0,
            max_vertex_output_components: 0,
            max_tessellation_generation_level: 0,
            max_tessellation_patch_size: 0,
            max_tessellation_control_per_vertex_input_components: 0,
            max_tessellation_control_per_vertex_output_components: 0,
            max_tessellation_control_per_patch_output_components: 0,
            max_tessellation_control_total_output_components: 0,
            max_tessellation_evaluation_input_components: 0,
            max_tessellation_evaluation_output_components: 0,
            max_geometry_shader_invocations: 0,
            max_geometry_input_components: 0,
            max_geometry_output_components: 0,
            max_geometry_output_vertices: 0,
            max_geometry_total_output_components: 0,
            max_fragment_input_components: 0,
            max_fragment_output_attachments: 0,
            max_fragment_dual_src_attachments: 0,
            max_fragment_combined_output_resources: 0,
            max_compute_shared_memory_size: 0,
            max_compute_work_group_count: [0; 3],
            max_compute_work_group_invocations: 0,
            max_compute_work_group_size: [0; 3],
            sub_pixel_precision_bits: 0,
            sub_texel_precision_bits: 0,
            mipmap_precision_bits: 0,
            max_draw_indexed_index_value: 0,
            max_draw_indirect_count: 0,
            max_sampler_lod_bias: 0.0,
            max_sampler_anisotropy: 0.0,
            max_viewports: 0,
            max_viewport_dimensions: [0; 2],
            viewport_bounds_range: [0.0; 2],
            viewport_sub_pixel_bits: 0,
            min_memory_map_alignment: 0,
            min_texel_buffer_offset_alignment: 0,
            min_uniform_buffer_offset_alignment: 0,
            min_storage_buffer_offset_alignment: 0,
            min_texel_offset: 0,
            max_texel_offset: 0,
            min_texel_gather_offset: 0,
            max_texel_gather_offset: 0,
            min_interpolation_offset: 0.0,
            max_interpolation_offset: 0.0,
            sub_pixel_interpolation_offset_bits: 0,
            max_framebuffer_width: 0,
            max_framebuffer_height: 0,
            max_framebuffer_layers: 0,
            framebuffer_color_sample_counts: SampleCountFlag::None,
            framebuffer_depth_sample_counts: SampleCountFlag::None,
            framebuffer_stencil_sample_counts: SampleCountFlag::None,
            framebuffer_no_attachments_sample_counts: SampleCountFlag::None,
            max_color_attachments: 0,
            sampled_image_color_sample_counts: SampleCountFlag::None,
            sampled_image_integer_sample_counts: SampleCountFlag::None,
            sampled_image_depth_sample_counts: SampleCountFlag::None,
            sampled_image_stencil_sample_counts: SampleCountFlag::None,
            storage_image_sample_counts: SampleCountFlag::None,
            max_sample_mask_words: 0,
            timestamp_compute_and_graphics: VK_FALSE,
            timestamp_period: 0.0,
            max_clip_distances: 0,
            max_cull_distances: 0,
            max_combined_clip_and_cull_distances: 0,
            discrete_queue_priorities: 0,
            point_size_range: [0.0; 2],
            line_width_range: [0.0; 2],
            point_size_granularity: 0.0,
            line_width_granularity: 0.0,
            strict_lines: VK_FALSE,
            standard_sample_locations: VK_FALSE,
            optimal_buffer_copy_offset_alignment: 0,
            optimal_buffer_copy_row_pitch_alignment: 0,
            non_coherent_atom_size: 0,
        }
    }
}

impl Default for PhysicalDeviceSparseProperties {
    fn default() -> Self {
        PhysicalDeviceSparseProperties {
            residency_standard_2d_block_shape: VK_FALSE,
            residency_standard_2d_multisample_block_shape: VK_FALSE,
            residency_standard_3d_block_shape: VK_FALSE,
            residency_aligned_mip_size: VK_FALSE,
            residency_non_resident_strict: VK_FALSE,
        }
    }
}

impl Default for PhysicalDeviceProperties {
    fn default() -> Self {
        PhysicalDeviceProperties {
            api_version: 0,
            driver_version: 0,
            vendor_id: 0,
            device_id: 0,
            device_type: PhysicalDeviceType::Other,
            device_name: [0; MAX_PHYSICAL_DEVICE_NAME_SIZE],
            pipeline_cache_uuid: [0; UUID_SIZE],
            limits: PhysicalDeviceLimits::default(),
            sparse_properties: PhysicalDeviceSparseProperties::default(),
        }
    }
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

impl Default for PhysicalDeviceFeatures {
    fn default() -> Self {
        PhysicalDeviceFeatures {
            s_type: StructureType::PhysicalDeviceFeatures,
            next: null_mut(),
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

impl PhysicalDeviceFeatures {
    pub fn default2(dynamic_render: *mut PhysicalDeviceDynamicRenderingFeatures) -> Self {
        PhysicalDeviceFeatures {
            s_type: StructureType::PhysicalDeviceFeatures,
            next: dynamic_render as *mut c_void,
            ..PhysicalDeviceFeatures::default()
        }
    }
}
